#include "net/tcp_listener_callbacks.h"

#include "net/address.h"
#include "net/memory.h"
#include "net/read_state_machine.h"
#include "net/tcp_client.h"
#include "net/tcp_client_callbacks.h"
#include "net/tcp_listener.h"

#include "log/error.h"
#include "log/logger.h"

#include "uthash.h"
#include "uv.h"

void
ts_tcp_listener_stop_cb(uv_handle_t *l)
{
    log_info("Listener stopped");
}

void
ts_tcp_listener_accepted_cb(uv_stream_t *stream, int status)
{
    int                     rc;
    struct ts_tcp_client *  client;
    struct ts_tcp_listener *listener;

    listener = (struct ts_tcp_listener *)stream->data;

    if ((rc = ts_tcp_client_create(&client)) != 0) {
        log_error("%s", ts_strerror(rc));
        return;
    }

    if ((rc = uv_accept(stream, (uv_stream_t *)&client->socket)) < 0) {
        log_error("%s", uv_strerror(rc));
        goto cleanup;
    }

    client->listener = listener;

    if ((rc = ts_tcp_client_start_read(client)) != 0) {
        log_error("ts_tcp_client_start_read: %s", ts_strerror(rc));
        goto cleanup;
    }

    HASH_ADD_INT(listener->clients, id, client);

    if (client->listener->app_cbs.on_connection_cb) {
        (*client->listener->app_cbs.on_connection_cb)(client);
    }

    return;

cleanup:
    ts_tcp_client_free(client);
}

void
ts_tcp_listener_disconnected_cb(uv_stream_t *stream)
{
    struct ts_tcp_client *client = (struct ts_tcp_client *)stream->data;

    if (client->listener->app_cbs.on_disconnection_cb) {
        (*client->listener->app_cbs.on_disconnection_cb)(client);
    }

    HASH_DEL(client->listener->clients, client);
    ts_tcp_client_free(client);
}
