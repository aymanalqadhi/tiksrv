#include "commands/command.h"

#include "commands/request.h"
#include "commands/response.h"

#include "log/error.h"
#include "log/logger.h"

#include <stdint.h>

static void
echo_command_exec(struct ts_request *req, struct ts_services_container *svcs)
{
    int                 rc;
    struct ts_response *resp;

    if ((rc = ts_respone_create(&resp)) != 0) {
        /* TODO:
         * Terminate the connection
         */
        log_error("ts_respone_create: %s", ts_strerror(rc));
        return;
    }

    if ((rc = ts_response_write(resp,
                                ts_request_get_buffer(req),
                                ts_request_get_length(req))) != 0) {
        log_error("ts_response_write: %s", ts_strerror(rc));
        return;
    }

    if ((rc = ts_response_commit(resp, req)) != 0) {
        log_error("ts_response_commit: %s", ts_strerror(rc));
        return;
    }
}

struct ts_command ts_echo_command = { .command = TS_COMMAND_ECHO,
                                      .func    = &echo_command_exec };
