#include "impl/tcp_client_callbacks.h"

#include "net/address.h"
#include "net/memory.h"
#include "net/read_state_machine.h"
#include "net/tcp_client.h"
#include "net/tcp_listener.h"

#include "log/error.h"
#include "log/logger.h"

#include "uv.h"

static void
free_listener_client(gpointer client)
{
    ts_tcp_client_close((struct ts_tcp_client *)client);
    ts_tcp_client_free((struct ts_tcp_client *)client);
}

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

    if (client->listener->app_cbs.on_connection_cb) {
        (*client->listener->app_cbs.on_connection_cb)(client);
    }

    if (!listener->clients) {
        listener->clients = g_hash_table_new_full(
            g_int_hash, g_int_equal, &g_free, &free_listener_client);
    }

    g_hash_table_insert(
        listener->clients,
        g_memdup((gconstpointer)&client->id, sizeof(client->id)),
        (gpointer)client);

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

    g_hash_table_remove(client->listener->clients, (gconstpointer)&client->id);
    ts_tcp_client_free(client);
}
