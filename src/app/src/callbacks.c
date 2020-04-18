#include "impl/callbacks.h"
#include "impl/shared.h"

#include "log/logger.h"
#include "net/address.h"
#include "net/message.h"

static inline void
display_request_info(const struct ts_request_message *req)
{
    log_trace("Got Message:");
    log_trace("[+] Command: %#08X", req->header->command);
    log_trace("[+] Flags: %#08X", req->header->flags);
    log_trace("[+] Sequence Number: %#08X", req->header->seq_number);
    log_trace("[+] Body Length: %#08X", req->header->body_length);
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
    int                rc;
    uint16_t           code;
    struct ts_command *cmd;

    display_request_info(req);

    if (!(cmd = ts_app_get_command(req->header->command))) {
        if ((rc = ts_tcp_client_respond_with_code(
                 client,
                 TS_RESPONSE_CODE_NO_SUCH_COMMAND,
                 req->header->seq_number)) != 0) {
            log_error("ts_tcp_client_respond_with_code: %s", ts_strerror(rc));
            ts_tcp_client_close(client);
            return;
        }
    }

    if ((rc = ts_command_execute(cmd, client, req)) != 0) {
        log_error("ts_command_execute: %s", ts_strerror(rc));
    }
}
