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

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_encode_response_header(const struct ts_response_header *resp,
                          void *                           buf,
                          size_t                           len)
{
    size_t pos = 0;

    CHECK_NULL_PARAMS_2(resp, buf);

    if (len < TS_MESSAGE_RESPONSE_HEADER_SIZE) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    memcpy(buf + pos, (void *)&resp->code, sizeof(resp->code));
    pos += sizeof(resp->code);
    memcpy(buf + pos, (void *)&resp->flags, sizeof(resp->flags));
    pos += sizeof(resp->flags);
    memcpy(buf + pos, (void *)&resp->seq_number, sizeof(resp->seq_number));
    pos += sizeof(resp->seq_number);
    memcpy(buf + pos, (void *)&resp->body_length, sizeof(resp->body_length));

    return TS_ERR_SUCCESS;
}
