#include "commands/command.h"

#include "commands/impl/request.h"
#include "commands/impl/response.h"

#include "net/memory.h"
#include "net/tcp_client.h"

#include "log/error.h"
#include "log/logger.h"
#include "services/container.h"
#include "util/validation.h"

#include <stdint.h>

extern struct ts_command ts_echo_command;

ts_error_t
ts_commands_export(ts_command_export_func_t export_func)
{
    (*export_func)(&ts_echo_command);

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_command_execute(struct ts_command *              cmd,
                   struct ts_tcp_client *           client,
                   const struct ts_request_message *req)
{
    int      rc;
    uint16_t code;

    struct ts_request  req_wrapper;
    struct ts_response resp_wrapper;

    CHECK_NULL_PARAMS_2(cmd, req);

    if ((rc = ts_request_init(&req_wrapper, client, req)) != 0) {
        log_error("ts_request_init: %s", ts_strerror(rc));
        return rc;
    }

    (*cmd->func)(&req_wrapper, NULL);
    return TS_ERR_SUCCESS;
}
