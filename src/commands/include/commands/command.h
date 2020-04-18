#ifndef TIKSRV_COMMNADS_COMMAND_H
#define TIKSRV_COMMNADS_COMMAND_H

#include "commands/request.h"
#include "commands/response.h"

#include "net/message.h"
#include "net/tcp_client.h"

#include "log/error.h"
#include "services/container.h"

#include "uthash.h"

#include <stdint.h>

enum ts_system_commands
{
    TS_COMMAND_ECHO
};

typedef uint16_t (*ts_command_func_t)(struct ts_request *,
                                      struct ts_response *,
                                      struct ts_services_container *);

struct ts_command
{
    uint32_t          command;
    ts_command_func_t func;
    UT_hash_handle    hh;
};

typedef void (*ts_command_exprt_func_t)(const struct ts_command *);

ts_error_t
ts_commands_export(ts_command_exprt_func_t export_func);

ts_error_t
ts_command_execute(struct ts_command *              cmd,
                   struct ts_tcp_client *           client,
                   const struct ts_request_message *req);

#endif
