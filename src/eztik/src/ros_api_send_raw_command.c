#include "eztik/commands.h"
#include "eztik/eztik.h"
#include "eztik/response.h"
#include "eztik/routeros/api.h"
#include "eztik/routeros/api_sentence.h"
#include "eztik/session.h"

#include "log/error.h"
#include "log/logger.h"

#include <glib.h>

#include <stdbool.h>

#define MAX_REQUEST_SIZE  0x1000
#define MAX_REQUEST_LINES 0x100

static void
api_send_raw_send_cb(bool                     success,
                     struct ros_api_sentence *sentence,
                     void *                   data)
{
    int                 rc;
    struct ts_response *resp;

    resp = (struct ts_response *)data;

    if (success) {
        log_info("Sent a raw API message from client #%u successfully",
                 ts_response_get_client_id(resp));
        ts_response_set_code(resp, TS_RESPONSE_CODE_SUCCESS);
    } else {
        log_error("Could not send a raw API message from client #%u",
                  ts_response_get_client_id);
        ts_response_set_type(resp, EZTIK_COMMANDS_TYPE);
        ts_response_set_code(resp, EZTIK_RESPONSE_ROS_API_WRITE_FAILED);
    }

    if ((rc = ts_response_commit(resp)) != 0) {
        log_error("ts_response_commit: %s", ts_strerror(rc));
    }

    ros_api_sentence_free(sentence);
    ts_response_free(resp);
}

static void
ros_api_send_raw_command_exec(const struct ts_request *req)
{
    int      rc;
    uint32_t client_id;
    char **  lines, **line_ptr;

    char     buf[MAX_REQUEST_SIZE];
    uint32_t buflen;

    struct eztik_session *   session;
    struct ts_response *     resp;
    struct ros_api_sentence *sentence;

    buflen    = sizeof(buf);
    client_id = ts_request_get_client_id(req);
    resp      = ts_respone_new(req);

    if (!(session = eztik_get_session(ts_request_get_client_id(req)))) {
        log_warn("Client #%u with no open session sent an API write request",
                 client_id);
        ts_response_set_type(resp, EZTIK_COMMANDS_TYPE);
        ts_response_set_code(resp, EZTIK_RESPONSE_SESSION_NOT_OPEN);
    } else {
        if (ts_request_get_length(req) == 0) {
            log_warn("Client #%u sent an empty API write request", client_id);
            ts_response_set_code(resp, TS_RESPONSE_CODE_SUCCESS);
        } else {
            if ((rc = ts_request_read_string(req, buf, &buflen)) != 0) {
                log_error("ts_request_read_string: %s", ts_strerror(rc));
                ts_response_set_code(resp, TS_RESPONSE_CODE_UNKNOWN_ERROR);
            } else {
                lines    = g_strsplit(buf, "\n", MAX_REQUEST_LINES);
                line_ptr = lines;
                sentence = ros_api_sentence_new(
                    *line_ptr, ts_request_get_sequence_number(req));

                while (*++line_ptr) {
                    ros_api_sentence_append(sentence, *line_ptr);
                }

                g_strfreev(lines);

                if ((rc = ros_api_send(session->api,
                                       sentence,
                                       api_send_raw_send_cb,
                                       (void *)resp)) == 0) {
                    return;
                } else {
                    log_error("ros_api_send: %s", ts_strerror(rc));
                }
            }
        }
    }

    if ((rc = ts_response_commit(resp)) != 0) {
        log_error("ts_response_commit: %s", ts_strerror(rc));
    }

    ts_response_free(resp);
}

struct ts_command eztik_ros_api_send_raw_command = {
    .command = EZTIK_COMMAND_ROS_API_SEND_RAW,
    .func    = &ros_api_send_raw_command_exec
};
