#include "commands/command.h"

#include "commands/request.h"
#include "commands/response.h"

#include <stdint.h>

static uint16_t
echo_command_exec(struct ts_request *           req,
                  struct ts_response *          resp,
                  struct ts_services_container *svcs)
{
    int rc;

    if ((rc = ts_response_write(resp,
                                ts_request_get_buffer(req),
                                ts_request_get_length(req))) != 0) {
        return TS_RESPONSE_CODE_UNKNOWN_ERROR;
    }

    return TS_RESPONSE_CODE_SUCCESS;
}

struct ts_command ts_echo_command = { .command = TS_COMMAND_ECHO,
                                      .func    = &echo_command_exec };
