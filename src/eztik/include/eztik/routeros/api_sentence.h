#ifndef EZTIK_ROUTEROS_API_SENTENCE_H
#define EZTIK_ROUTEROS_API_SENTENCE_H

#include <glib.h>

#include <stdint.h>

struct ros_api_sentence
{
    GSList * words;
    uint32_t tag;
};

/*!
 * \brief Creates a new RouterOS API sentence object with a command of \see
 *        command and a tag of \see tag
 *
 * \param [in] command  The sentence command word
 * \param [in] tag      The sentence tag parameter value
 *
 * \return The created sentence object
 */
struct ros_api_sentence *
ros_api_sentence_new(const char *command, uint32_t tag);

/*!
 * \brief Appends a word to a RouterOS API sentence object pointed to by \see s
 *
 * \param [in, out] s     A pointer to the sentence object into which to append
 * \param [in]      word  The word string to be appended
 */
void
ros_api_sentence_append(struct ros_api_sentence *s, const char *word);

/*!
 * \brief Appends a formatted word to a RouterOS API sentence object pointed to
 *        by \see s
 *
 * \param [in, out] s    A pointer to the sentence object into which to append
 * \param [in]      fmt  The word format which to be used to form the word
 *                       string
 */
void
ros_api_sentence_appendf(struct ros_api_sentence *s, const char *fmt, ...);

/*!
 * \brief Appends an attribute word to a RouterOS API sentence object pointed to
 *        by \see s
 *
 * \param [in, out] s    A pointer to the sentence object into which to append
 * \param [in]      key  The parameter key
 * \param [in]      key  The parameter value
 */
void
ros_api_sentence_append_attr(struct ros_api_sentence *s,
                             const char *             key,
                             const char *             value);

/*!
 * \brief Falattens a RouterOS API sentence object to a linear byte buffer with
 *        the apropriate encoding for words and their lengths
 *
 * \param [in] s  A pointer to the sentence whcih to be flattened
 *
 * \return The flattened sentence byte buffer
 */
GByteArray *
ros_api_sentence_flatten(const struct ros_api_sentence *s);

/*
 * \brief Frees the used resources by a RouterOS API sentence object
 *
 * \param [in, out] s  A pointer to the sentence object which to be freed
 */
void
ros_api_sentence_free(struct ros_api_sentence *s);

#endif
