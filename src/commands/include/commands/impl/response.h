#ifndef TIKSRV_COMMANDS_IMPL_RESPONSE_H
#define TIKSRV_COMMANDS_IMPL_RESPONSE_H

#include <stdint.h>

struct ts_command_response
{
    uint16_t flags;
    void *   body_buffer;
    uint32_t buffer_length, buffer_capacity;
};

#endif
