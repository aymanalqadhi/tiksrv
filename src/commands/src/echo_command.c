#include "commands/command.h"

#include "commands/request.h"
#include "commands/response.h"

#include <stdint.h>

static uint16_t
echo_command_exec(struct ts_command_request *   req,
                  struct ts_command_response *  resp,
                  struct ts_services_container *svcs)
{
    ts_response_write(resp, req->body_buffer, req->body_length);
    return 0;
}

struct ts_command ts_echo_command = { .id   = TS_COMMAND_ECHO,
                                      .func = &echo_command_exec };
