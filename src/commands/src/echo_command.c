#include "commands/command.h"

#include "commands/request.h"
#include "commands/response.h"

#include "log/error.h"
#include "log/logger.h"

#include <stdint.h>

static void
echo_command_exec(const struct ts_request *req)
{
    int                 rc;
    struct ts_response *resp;

    resp = ts_respone_new(req);

    ts_response_write(
        resp, ts_request_get_buffer(req), ts_request_get_length(req));

    if ((rc = ts_response_commit(resp)) != 0) {
        log_error("ts_response_commit: %s", ts_strerror(rc));
    }

    ts_response_free(resp);
}

struct ts_command ts_echo_command = { .command = TS_COMMAND_ECHO,
                                      .func    = &echo_command_exec };
