#include "net/message.h"

#include "log/error.h"
#include "util/validation.h"

#include <endian.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APPEND_TO_BUFFER(buf, item)                                            \
    memcpy(buf, (const void *)&(item), sizeof(item));                          \
    buf += sizeof(item);

#define EXTRACT_FROM_BUFFER(buf, item)                                         \
    memcpy((void *)&(item), buf, sizeof(item));                                \
    buf += sizeof(item);

#define BE_EXTRACT_FROM_BUFFER(buf, item, sz)                                  \
    EXTRACT_FROM_BUFFER(buf, item);                                            \
    item = be##sz##toh(item);

ts_error_t
ts_encode_request_header(const struct ts_request_header *req,
                         void *                          buf,
                         size_t                          len)
{
    uint32_t command, flags, seq_number, body_length;

    CHECK_NULL_PARAMS_2(req, buf);

    if (len < TS_MESSAGE_REQUEST_HEADER_SIZE) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    command     = htobe32(req->command);
    flags       = htobe32(req->flags);
    seq_number  = htobe32(req->seq_number);
    body_length = htobe32(req->body_length);

    APPEND_TO_BUFFER(buf, command);
    APPEND_TO_BUFFER(buf, flags);
    APPEND_TO_BUFFER(buf, seq_number);
    APPEND_TO_BUFFER(buf, body_length);

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_decode_request_header(struct ts_request_header *req,
                         const void *              buf,
                         size_t                    len)
{
    CHECK_NULL_PARAMS_2(req, buf);

    if (len < TS_MESSAGE_REQUEST_HEADER_SIZE) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    BE_EXTRACT_FROM_BUFFER(buf, req->command, 32);
    BE_EXTRACT_FROM_BUFFER(buf, req->flags, 32);
    BE_EXTRACT_FROM_BUFFER(buf, req->seq_number, 32);
    BE_EXTRACT_FROM_BUFFER(buf, req->body_length, 32);

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_encode_response_header(const struct ts_response_header *resp,
                          void *                           buf,
                          size_t                           len)
{
    uint16_t code;
    uint32_t flags, seq_number, body_length;

    CHECK_NULL_PARAMS_2(resp, buf);

    if (len < TS_MESSAGE_RESPONSE_HEADER_SIZE) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    code        = htobe16(resp->code);
    flags       = htobe32(resp->flags);
    seq_number  = htobe32(resp->seq_number);
    body_length = htobe32(resp->body_length);

    APPEND_TO_BUFFER(buf, code);
    APPEND_TO_BUFFER(buf, flags);
    APPEND_TO_BUFFER(buf, seq_number);
    APPEND_TO_BUFFER(buf, body_length);

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_decode_response_header(struct ts_response_header *resp,
                          const void *               buf,
                          size_t                     len)
{
    size_t pos = 0;

    CHECK_NULL_PARAMS_2(resp, buf);

    if (len < TS_MESSAGE_RESPONSE_HEADER_SIZE) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    BE_EXTRACT_FROM_BUFFER(buf, resp->code, 16);
    BE_EXTRACT_FROM_BUFFER(buf, resp->flags, 32);
    BE_EXTRACT_FROM_BUFFER(buf, resp->seq_number, 32);
    BE_EXTRACT_FROM_BUFFER(buf, resp->body_length, 32);

    return TS_ERR_SUCCESS;
}
