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
api_open_session_new_cb(struct eztik_session *session, void *data)
{
    int                 rc;
    struct ts_response *resp;

    resp = (struct ts_response *)data;

    if (!session) {
        log_info("Could not open a session for client #%u",
                 ts_response_get_client_id(resp));
        ts_response_set_code(resp, EZTIK_RESPONSE_SESSION_OPEN_FAILED);
        if ((rc = ts_response_commit(resp)) != 0) {
            log_error("ts_response_commit: %s", ts_strerror(rc));
        }
        goto cleanup;
    }

    ts_response_set_code(resp, TS_RESPONSE_CODE_SUCCESS);
    if ((rc = ts_response_commit(resp)) != 0) {
        log_error("ts_response_commit: %s", ts_strerror(rc));
        eztik_session_free(session);
    } else {
        log_info("A RouterOS session was opened successfully for client #%u",
                 ts_response_get_client_id(resp));
        eztik_add_session(session);
    }

cleanup:
    ts_response_free(resp);
}

static void
ros_api_open_command_exec(const struct ts_request *req)
{
    int      rc;
    uint32_t client_id;

    struct ts_response *  resp;
    struct eztik_session *session;

    client_id = ts_request_get_client_id(req);
    resp      = ts_respone_new(req);

    if (eztik_has_session(client_id)) {
        log_warn(
            "Client #%u with an already open session requested a new session",
            client_id);
        ts_response_set_code(resp, EZTIK_RESPONSE_SESSION_ALREADY_OPEN);
        if ((rc = ts_response_commit(resp)) != 0) {
            log_error("ts_response_commit: %s", ts_strerror(rc));
        }
        ts_response_free(resp);
        return;
    }

    log_norm("Creating a new session for client #%u", client_id);
    eztik_session_new(client_id, &api_open_session_new_cb, (void *)resp);
}

struct ts_command eztik_ros_api_open_command = {
    .command = EZTIK_COMMAND_ROS_API_OPEN,
    .func    = &ros_api_open_command_exec
};
