#ifndef TIKSRV_COMMANDS_IMPL_REQUEST_H
#define TIKSRV_COMMANDS_IMPL_REQUEST_H

#include <stdint.h>

struct ts_command_request
{
    uint32_t    client_id;
    uint16_t    flags;
    const void *body_buffer;
    uint32_t    body_length, current_pos;
};

#endif
