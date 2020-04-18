#include "commands/impl/request.h"
#include "commands/request.h"

#include "net/message.h"
#include "net/tcp_client.h"

#include "log/error.h"
#include "util/validation.h"

#include <endian.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define REQUEST_READ_IMPL(size)                                                \
    ts_error_t ts_request_read_uint##size(struct ts_request *req,              \
                                          uint##size##_t *   valptr)           \
    {                                                                          \
        int            rc;                                                     \
        uint##size##_t tmp;                                                    \
                                                                               \
        CHECK_NULL_PARAMS_1(valptr);                                           \
                                                                               \
        if ((rc = ts_request_read(req, (void *)&tmp, sizeof(tmp))) != 0) {     \
            return rc;                                                         \
        }                                                                      \
                                                                               \
        *valptr = be##size##toh(tmp);                                          \
        return TS_ERR_SUCCESS;                                                 \
    }

ts_error_t
ts_request_read(struct ts_request *req, void *buf, size_t len)
{
    CHECK_NULL_PARAMS_2(req, buf);
    CHECK_UNINITIALIZED_DATA_1(req->message);

    if (len + req->current_pos > req->message->header->body_length) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    memcpy(buf, req->message->body + req->current_pos, len);
    req->current_pos += len;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_request_read_byte(struct ts_request *req, uint8_t *valptr)
{
    CHECK_NULL_PARAMS_2(req, valptr);
    CHECK_NULL_PARAMS_1(req->message);

    if (req->current_pos >= req->message->header->body_length) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    *valptr = *(uint8_t *)(req->message->body + req->current_pos++);
    return TS_ERR_SUCCESS;
}

REQUEST_READ_IMPL(16)
REQUEST_READ_IMPL(32)
REQUEST_READ_IMPL(64)

ts_error_t
ts_request_init(struct ts_request *req,
        struct ts_tcp_client *client,
        const struct ts_request_message *message)
{
    CHECK_NULL_PARAMS_1(req);

    req->client = client;
    req->message = message;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_request_read_string(struct ts_request *req, char *buf, uint32_t *len)
{
    int      rc;
    uint32_t to_read;

    CHECK_NULL_PARAMS_2(buf, len);

    if ((rc = ts_request_read_uint32(req, &to_read)) != 0) {
        return rc;
    }

    to_read = to_read < *len ? to_read : *len;

    if ((rc = ts_request_read(req, (void *)buf, to_read)) != 0) {
        return rc;
    }

    buf[(*len = to_read)] = '\0';
    return TS_ERR_SUCCESS;
}

uint32_t
ts_request_get_client_id(const struct ts_request *req)
{
    return req->client->id;
}

uint16_t
ts_request_get_flags(const struct ts_request *req)
{
    return req->message->header->flags;
}

const void *
ts_request_get_buffer(const struct ts_request *req)
{
    return req->message->body;
}

uint32_t
ts_request_get_length(const struct ts_request *req)
{
    return req->message->header->body_length;
}

uint32_t
ts_request_get_sequence_number(const struct ts_request *req)
{
    return req->message->header->seq_number;
}
