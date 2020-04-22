#include "commands/response.h"

#include "commands/impl/request.h"
#include "commands/impl/response.h"

#include "log/error.h"
#include "log/logger.h"
#include "util/validation.h"

#include <glib.h>

#include <assert.h>
#include <endian.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline size_t
calculate_new_capacity(size_t current_cap, size_t current_len, size_t n)
{
    return pow(2, ceil(log2(current_len + n)));
}

static inline void
reserve_bytes(struct ts_response *resp, size_t n)
{
    void * tmp;
    size_t new_cap;

    assert(resp != NULL);

    if (resp->buffer_length + n <= resp->buffer_capacity) {
        return;
    }

    new_cap =
        calculate_new_capacity(resp->buffer_capacity, resp->buffer_length, n);
    tmp = resp->body_buffer ? g_realloc(resp->body_buffer, new_cap)
                            : g_malloc(new_cap);

    resp->buffer_capacity = new_cap;
    resp->body_buffer     = tmp;
}

void
ts_response_write(struct ts_response *resp, const void *buf, uint32_t len)
{
    int rc;

    assert(resp != NULL && buf != NULL);

    if (len == 0) {
        return;
    }

    reserve_bytes(resp, len);

    memcpy(resp->body_buffer + resp->buffer_length, buf, len);
    resp->buffer_length += len;
}

void
ts_response_write_byte(struct ts_response *resp, uint8_t value)
{
    int rc;

    assert(resp != NULL);

    reserve_bytes(resp, 1);
    *((uint8_t *)resp->body_buffer + resp->buffer_length++) = value;
}

void
ts_response_write_uint16(struct ts_response *resp, uint16_t value)
{
    assert(resp != NULL);

    value = htobe16(value);
    ts_response_write(resp, (void *)&value, sizeof(value));
}

void
ts_response_write_uint32(struct ts_response *resp, uint32_t value)
{
    assert(resp != NULL);

    value = htobe32(value);
    ts_response_write(resp, (void *)&value, sizeof(value));
}

void
ts_response_write_uint64(struct ts_response *resp, uint64_t value)
{
    assert(resp != NULL);

    value = htobe64(value);
    ts_response_write(resp, (void *)&value, sizeof(value));
}

void
ts_response_write_nstring(struct ts_response *resp,
                          const char *        str,
                          uint32_t            len)
{
    assert(resp != NULL && str != NULL);

    ts_response_write_uint32(resp, len);
    ts_response_write(resp, (const void *)str, len);
}

ts_error_t
ts_response_commit(struct ts_response *resp)
{
    int rc;

    struct ts_response_message resp_message;
    struct ts_response_header  resp_header;

    resp_header.code        = resp->code;
    resp_header.seq_number  = resp->seq_number;
    resp_header.flags       = resp->flags << 16;
    resp_header.body_length = resp->buffer_length;

    resp_message.header = &resp_header;
    resp_message.body   = resp->body_buffer;

    if ((rc = ts_tcp_client_respond(resp->client, &resp_message)) != 0) {
        log_error("ts_tcp_client_respond: %s", ts_strerror(rc));
        ts_tcp_client_close(resp->client);
        return rc;
    }

    memset(resp, 0, sizeof(*resp));
    return TS_ERR_SUCCESS;
}

void
ts_response_write_string(struct ts_response *resp, const char *str)
{
    ts_response_write_nstring(resp, str, strlen(str));
}

struct ts_response *
ts_respone_new(const struct ts_request *req)
{
    struct ts_response *ret = g_new0(struct ts_response, 1);
    ret->client             = req->client;
    ret->seq_number         = req->message->header->seq_number;

    return ret;
}

void
ts_response_free(struct ts_response *resp)
{
    if (resp->body_buffer) {
        g_free(resp->body_buffer);
    }
    g_free(resp);
}

uint16_t
ts_response_get_code(const struct ts_response *resp)
{
    assert(resp != NULL);
    return resp->code;
}

uint32_t
ts_response_get_flags(const struct ts_response *resp)
{
    assert(resp != NULL);
    return resp->flags;
}

const void *
ts_response_get_buffer(const struct ts_response *resp)
{
    assert(resp != NULL);
    return resp->body_buffer;
}

uint32_t
ts_response_get_length(const struct ts_response *resp)
{
    assert(resp != NULL);
    return resp->buffer_length;
}

uint32_t
ts_response_get_capacity(const struct ts_response *resp)
{
    assert(resp != NULL);
    return resp->buffer_capacity;
}

void
ts_response_set_code(struct ts_response *resp, uint16_t code)
{
    assert(resp != NULL);
    resp->code = code;
}

void
ts_response_set_flags(struct ts_response *resp, uint32_t flags)
{
    assert(resp != NULL);
    resp->flags = flags;
}
