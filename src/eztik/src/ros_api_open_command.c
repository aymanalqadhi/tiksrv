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

struct api_open_context
{
    struct ts_response *  response;
    struct eztik_session *session;
};

static void
api_open_cb(bool success, void *data)
{
    int                      rc;
    struct api_open_context *ctx;

    ctx = (struct api_open_context *)data;

    if (success) {
        log_info("A RouterOS session was opened successfully for client #%u",
                 ctx->session->id);
        ts_response_set_code(ctx->response, TS_RESPONSE_CODE_SUCCESS);
        if ((rc = ts_response_commit(ctx->response)) != 0) {
            log_error("ts_response_commit: %s", ts_strerror(rc));
            eztik_session_close(ctx->session);
            eztik_session_free(ctx->session);
        } else {
            eztik_add_session(ctx->session);
        }
    } else {
        log_info("Could not open a session for client #%u", ctx->session->id);
        ts_response_set_code(ctx->response, TS_ERR_SOCKET_OPEN_FAILED);
        if ((rc = ts_response_commit(ctx->response)) != 0) {
            log_error("ts_response_commit: %s", ts_strerror(rc));
        }
        eztik_session_free(ctx->session);
    }

    ts_response_free(ctx->response);
    g_free(ctx);
}

static void
ros_api_open_command_exec(const struct ts_request *req)
{
    int      rc;
    uint32_t client_id;

    struct ts_response *     resp;
    struct eztik_session *   session;
    struct api_open_context *ctx;

    client_id = ts_request_get_client_id(req);
    resp      = ts_respone_new(req);

    if (eztik_has_session(client_id)) {
        log_warn(
            "Client #%u with an already open session requested a new session",
            client_id);
        ts_response_set_code(resp, EZTIK_REPONSE_SESSION_ALREADY_OPEN);
        if ((rc = ts_response_commit(resp)) != 0) {
            log_error("ts_response_commit: %s", ts_strerror(rc));
        }
        ts_response_free(resp);
        return;
    }

    log_norm("Creating a new session for client  #%u", client_id);

    session       = eztik_session_new(client_id);
    ctx           = g_new(struct api_open_context, 1);
    ctx->response = resp;
    ctx->session  = session;

    if ((rc = ros_api_open(&session->api, &api_open_cb, (void *)ctx)) != 0) {
        log_error("ros_api_open: %s", ts_strerror(rc));
        ts_response_set_code(resp, TS_RESPONSE_CODE_UNKNOWN_ERROR);
        if ((rc = ts_response_commit(resp)) != 0) {
            log_error("ts_response_commit: %s", ts_strerror(rc));
        }
        ts_response_free(resp);
        eztik_session_free(session);
        g_free(ctx);
    }
}

struct ts_command eztik_ros_api_open_command = {
    .command = EZTIK_COMMAND_ROS_API_OPEN,
    .func    = &ros_api_open_command_exec
};
