#ifndef TIKSRV_NET_MESSAGE_H
#define TIKSRV_NET_MESSAGE_H

#include "error.h"

#include <stddef.h>
#include <stdint.h>

#define TS_MESSAGE_REQUEST_HEADER_SIZE  16
#define TS_MESSAGE_RESPONSE_HEADER_SIZE 14

struct ts_request_header
{
    uint32_t command;
    uint32_t flags;
    uint32_t seq_number;
    uint32_t body_length;
};

struct ts_response_header
{
    uint16_t code;
    uint32_t flags;
    uint32_t seq_number;
    uint32_t body_length;
};

struct ts_request_message
{
    const struct ts_request_header *header;
    const void *                    body;
};

struct tsp_response_message
{
    const struct ts_response_header *header;
    const void *                     body;
};

ts_error_t
ts_decode_request_header(struct ts_request_header *req,
                         const void *              buf,
                         size_t                    len);

ts_error_t
ts_encode_response_header(const struct ts_response_header *resp,
                          void *                           buf,
                          size_t                           len);

#endif
