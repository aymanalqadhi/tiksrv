#include "net/tcp_listener_callbacks.h"

#include "net/address.h"
#include "net/memory.h"
#include "net/tcp_client.h"
#include "net/tcp_client_callbacks.h"
#include "net/tcp_listener.h"

#include "error.h"
#include "log.h"

#include <uthash.h>
#include <uv.h>

void
ts_tcp_listener_stop_cb(uv_handle_t *l)
{
    log_info("Listener closed");
}

void
ts_tcp_listener_accept_cb(uv_stream_t *l, int status)
{
    int                     rc;
    char                    addrbuf[INET6_ADDRSTRLEN];
    struct ts_tcp_client *  client;
    struct ts_tcp_listener *listener;

    listener = (struct ts_tcp_listener *)l->data;

    if ((rc = ts_tcp_client_create(&client)) != 0) {
        log_error("%s", ts_strerror(rc));
        return;
    }

    if ((rc = uv_accept(l, (uv_stream_t *)&client->socket)) < 0) {
        log_error("%s", uv_strerror(rc));
        goto cleanup;
    }

    client->listener = listener;

    if ((rc = ts_addr_to_string(&client->socket, addrbuf, sizeof(addrbuf))) !=
        0) {
        log_error("ts_addr_to_string: %s", ts_strerror(rc));
        goto cleanup;
    }

    if ((rc = uv_read_start((uv_stream_t *)&client->socket,
                            &ts_header_buffer_alloc_cb,
                            &ts_tcp_client_header_read_cb)) < 0) {
        log_error("uv_read_start: %s", uv_strerror(rc));
        goto cleanup;
    }

    HASH_ADD_INT(listener->clients, id, client);

    log_info(
        "Got connection from: %s, and was assigned #%d", addrbuf, client->id);
    return;

cleanup:
    ts_tcp_client_free(client);
}

void
ts_tcp_listener_disconnect_cb(uv_stream_t *s)
{
    struct ts_tcp_client *client = (struct ts_tcp_client *)s->data;

    log_info("Connection to client #%d was closed", client->id);
    HASH_DEL(client->listener->clients, client);
}
