#include "app/callbacks.h"
#include "app/shared.h"

#include "commands/impl/request.h"
#include "commands/impl/response.h"
#include "commands/request.h"
#include "commands/response.h"

#include "log/logger.h"
#include "net/address.h"
#include "net/message.h"
#include "util/memory.h"

static inline void
display_request_info(const struct ts_request_message *req)
{
    log_trace("Got Message:");
    log_trace("[+] Command: %#08X", req->header->command);
    log_trace("[+] Flags: %#08X", req->header->flags);
    log_trace("[+] Sequence Number: %#08X", req->header->seq_number);
    log_trace("[+] Body Length: %#08X", req->header->body_length);
}

static inline void
make_request_wrapper(struct ts_command_request *      req_wrapper,
                     const struct ts_request_message *req,
                     uint32_t                         client_id)
{
    req_wrapper->client_id   = client_id;
    req_wrapper->body_buffer = req->body;
    req_wrapper->body_length = req->header->body_length;
    req_wrapper->current_pos = 0;
    req_wrapper->flags       = 0;
}

void
ts_on_client_connection(struct ts_tcp_client *client)
{
    int  rc;
    char addrbuf[INET6_ADDRSTRLEN];

    if ((rc = ts_addr_to_string(&client->socket, addrbuf, sizeof(addrbuf))) !=
        0) {
        log_error("ts_addr_to_string: %s", ts_strerror(rc));
        return;
    }

    log_info(
        "Got connection from: %s, and was assigned #%d", addrbuf, client->id);
}

void
ts_on_client_disconnection(struct ts_tcp_client *client)
{
    log_info("Connection to client #%d was closed", client->id);
}

void
ts_on_client_request(struct ts_tcp_client *           client,
                     const struct ts_request_message *req)
{
    int      rc;
    uint16_t code;

    struct ts_response_message resp;
    struct ts_response_header  resp_header;
    struct ts_command_request  req_wrapper;
    struct ts_command_response resp_wrapper;
    struct ts_command *        cmd;

    display_request_info(req);

    if (!(cmd = ts_app_get_command(req->header->command))) {
        if ((rc = ts_tcp_client_respond_with_code(
                 client,
                 TS_COMMAND_CODE_NO_SUCH_COMMAND,
                 req->header->seq_number)) != 0) {
            log_error("ts_tcp_client_respond_with_code: %s", ts_strerror(rc));
            ts_tcp_client_close(client);
            return;
        }
    }

    make_request_wrapper(&req_wrapper, req, client->id);
    memset(&resp_wrapper, 0, sizeof(resp_wrapper));

    code = (*cmd->func)(&req_wrapper, &resp_wrapper, NULL);

    resp_header.code        = code;
    resp_header.seq_number  = req->header->seq_number;
    resp_header.flags       = resp_wrapper.flags << 16;
    resp_header.body_length = resp_wrapper.buffer_length;

    resp.header = &resp_header;
    resp.body   = resp_wrapper.body_buffer;

    if ((rc = ts_tcp_client_respond(client, &resp)) != 0) {
        log_error("ts_tcp_client_respond: %s", ts_strerror(rc));
        ts_tcp_client_close(client);
    }
}
