#ifndef TIKSRV_COMMANDS_IMPL_REQUEST_H
#define TIKSRV_COMMANDS_IMPL_REQUEST_H

#include "net/message.h"
#include "net/tcp_client.h"

#include <stdint.h>

struct ts_request
{
    uint32_t current_pos;

    struct ts_tcp_client *           client;
    const struct ts_request_message *message;
};

#endif
