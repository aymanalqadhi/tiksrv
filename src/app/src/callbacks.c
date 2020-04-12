#include "app/callbacks.h"

#include "log/logger.h"
#include "net/address.h"
#include "net/message.h"

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
    log_trace("Got Message:");
    log_trace("[+] Command: %#08X", req->header->command);
    log_trace("[+] Flags: %#08X", req->header->flags);
    log_trace("[+] Sequence Number: %#08X", req->header->seq_number);
    log_trace("[+] Body Length: %#08X", req->header->body_length);
    log_trace("[+] Message Body:");
    fwrite(req->body, 1, req->header->body_length, stdout);
    putchar('\n');
}
