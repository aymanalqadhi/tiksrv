#include "eztik/routeros/api_sentence.h"

#include <glib.h>

#include <assert.h>
#include <endian.h>
#include <stdarg.h>
#include <stdint.h>

static void
encode_length(GByteArray *array, uint32_t len)
{
    uint8_t tmp;

    assert(array != NULL);

    if (len < 0x80) {
        tmp = len & 0xFF;
        g_byte_array_append(array, &tmp, 1);
    } else if (len < 0x4000) {
        len = htobe32(((len | 0x8000) & 0x0000FFFF) << 0x10);
        g_byte_array_append(array, (const uint8_t *)&len, 2);
    } else if (len < 0x200000) {
        len = htobe32(((len | 0xC00000) & 0x00FFFFFF) << 0x8);
        g_byte_array_append(array, (const uint8_t *)&len, 3);
    } else if (len < 0x10000000) {
        len = htobe32(len | 0xE0000000);
        g_byte_array_append(array, (const uint8_t *)&len, 4);
    } else {
        tmp = 0xF0;
        len = htobe32(len);
        g_byte_array_append(array, &tmp, 1);
        g_byte_array_append(array, (const uint8_t *)&len, 4);
    }
}

static void
word_free_cb(gpointer ptr)
{
    assert(ptr != NULL);
    g_string_free((GString *)ptr, TRUE);
}

struct ros_api_sentence *
ros_api_sentence_new(const char *command, uint32_t tag)
{
    struct ros_api_sentence *sentence;

    sentence        = g_new(struct ros_api_sentence, 1);
    sentence->tag   = tag;
    sentence->words = g_array_new(FALSE, FALSE, sizeof(GString *));
    g_array_set_clear_func(sentence->words, &word_free_cb);

    ros_api_sentence_append(sentence, command);
    ros_api_sentence_appendf(sentence, "tag=%u", tag);

    return sentence;
}

void
ros_api_sentence_append(struct ros_api_sentence *s, const char *word)
{
    GString *str;

    assert(s != NULL);
    assert(word != NULL);

    str = g_string_new(word);
    g_array_append_val(s->words, str);
}

void
ros_api_sentence_appendf(struct ros_api_sentence *s, const char *fmt, ...)
{
    gchar * str;
    va_list args;

    assert(s != NULL);
    assert(fmt != NULL);

    va_start(args, fmt);
    str = g_strdup_vprintf(fmt, args);
    ros_api_sentence_append(s, str);
    g_free(str);
    va_end(args);
}

void
ros_api_sentence_append_attr(struct ros_api_sentence *s,
                             const char *             key,
                             const char *             value)
{
    assert(s != NULL);
    assert(key != NULL);
    assert(value != NULL);

    ros_api_sentence_appendf(s, "%s=%s", key, value);
}

GByteArray *
ros_api_sentence_flatten(struct ros_api_sentence *s)
{
    int         i;
    GByteArray *ret;
    GString *   word;

    assert(s != NULL);

    ret = g_byte_array_new();

    for (i = 0; i < s->words->len; ++i) {
        word = g_array_index(s->words, GString *, i);
        encode_length(ret, word->len);
        g_byte_array_append(ret, word->str, word->len);
    }
    encode_length(ret, 0);

    return ret;
}

void
ros_api_free(struct ros_api_sentence *s)
{
    assert(s != NULL);

    g_array_free(s->words, TRUE);
    g_free(s);
}
