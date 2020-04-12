#include "net/message.h"

#include "error.h"
#include "utils/validation.h"

#include <endian.h>
#include <stddef.h>
#include <string.h>

ts_error_t
ts_decode_request_header(struct ts_request_header *req,
                         const void *              buf,
                         size_t                    len)
{
    size_t pos = 0;

    CHECK_NULL_PARAMS_2(req, buf);

    if (len < TS_MESSAGE_REQUEST_HEADER_SIZE) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    memcpy((void *)&req->command, buf + pos, sizeof(req->command));
    pos += sizeof(req->command);
    memcpy((void *)&req->flags, buf + pos, sizeof(req->flags));
    pos += sizeof(req->flags);
    memcpy((void *)&req->seq_number, buf + pos, sizeof(req->seq_number));
    pos += sizeof(req->seq_number);
    memcpy((void *)&req->body_length, buf + pos, sizeof(req->body_length));

    req->command     = be32toh(req->command);
    req->flags       = be32toh(req->flags);
    req->seq_number  = be32toh(req->seq_number);
    req->body_length = be32toh(req->body_length);

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_encode_response_header(const struct ts_response_header *resp,
                          void *                           buf,
                          size_t                           len)
{
    size_t   pos;
    uint16_t code;
    uint32_t flags, seq_number, body_length;

    pos = 0;

    code        = htobe16(resp->code);
    flags       = htobe32(resp->flags);
    seq_number  = htobe32(resp->seq_number);
    body_length = htobe32(resp->body_length);

    CHECK_NULL_PARAMS_2(resp, buf);

    if (len < TS_MESSAGE_RESPONSE_HEADER_SIZE) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    memcpy(buf + pos, (void *)&code, sizeof(code));
    pos += sizeof(code);
    memcpy(buf + pos, (void *)&flags, sizeof(flags));
    pos += sizeof(flags);
    memcpy(buf + pos, (void *)&seq_number, sizeof(seq_number));
    pos += sizeof(seq_number);
    memcpy(buf + pos, (void *)&body_length, sizeof(body_length));

    return TS_ERR_SUCCESS;
}
