#include "eztik/commands.h"
#include "eztik/eztik.h"
#include "eztik/response.h"
#include "eztik/session.h"

#include "eztik/routeros/api.h"

#include "commands/command.h"
#include "commands/request.h"
#include "commands/response.h"
#include "log/error.h"
#include "log/logger.h"

#include <stdbool.h>

static void
ros_api_close_command_exec(const struct ts_request *req)
{
    int                 rc;
    uint32_t            client_id;
    struct ts_response *resp;

    client_id = ts_request_get_client_id(req);
    resp      = ts_respone_new(req);

    if (eztik_has_session(client_id)) {
        log_info("Closing session for client #%u", client_id);
        eztik_remove_session(client_id);
        ts_response_set_code(resp, TS_RESPONSE_CODE_SUCCESS);
    } else {
        log_warn("Client #%u with no open session sent a session close command",
                 client_id);
        ts_response_set_type(resp, EZTIK_COMMANDS_TYPE);
        ts_response_set_code(resp, EZTIK_RESPONSE_SESSION_NOT_OPEN);
    }

    if ((rc = ts_response_commit(resp)) != 0) {
        log_error("ts_response_commit: %s", ts_strerror(rc));
    }
    ts_response_free(resp);
}

struct ts_command eztik_ros_api_close_command = {
    .command = EZTIK_COMMAND_ROS_API_CLOSE,
    .func    = &ros_api_close_command_exec
};
