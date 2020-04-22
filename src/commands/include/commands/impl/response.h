#ifndef TIKSRV_COMMANDS_IMPL_RESPONSE_H
#define TIKSRV_COMMANDS_IMPL_RESPONSE_H

#include "net/tcp_client.h"

#include <stdint.h>

struct ts_response
{
    uint16_t code;
    uint32_t flags;
    uint32_t seq_number;
    void *   body_buffer;
    uint32_t buffer_length, buffer_capacity;

    struct ts_tcp_client *client;
};

#endif
