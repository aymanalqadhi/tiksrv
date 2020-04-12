#include "app.h"

#include "config.h"
#include "error.h"
#include "log.h"

#include "net/address.h"
#include "net/tcp_client.h"
#include "net/tcp_listener.h"

#include <stdlib.h>

static struct ts_tcp_listener *listener;

static void
on_client_connection(struct ts_tcp_client *client)
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

static void
on_client_disconnection(struct ts_tcp_client *client)
{
    log_info("Connection to client #%d was closed", client->id);
}

static void
on_client_request(struct ts_tcp_client *client)
{
    log_fatal("Got request from: #%d\n", client->id);
    printf("[APP] Got Message: ");
    fwrite(client->read_ctx.buf, 1, client->read_ctx.nread, stdout);
    putchar('\n');
}

static ts_error_t
app_init(const struct ts_config *cfg)
{
    int rc;

    log_info("Creating a TCP listener on port %u...", cfg->listen_port);
    if ((rc = ts_tcp_listener_create(&listener, cfg)) != 0) {
        return rc;
    }

    listener->on_connection_cb    = &on_client_connection;
    listener->on_disconnection_cb = &on_client_disconnection;
    listener->on_request_cb       = &on_client_request;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_app_run(const struct ts_config *cfg)
{
    int rc;

    log_info("Initializing app");
    if ((rc = app_init(cfg)) != 0) {
        return rc;
    }

    log_info("Starting listener");
    if ((rc = ts_tcp_listener_start(listener)) != 0) {
        return rc;
    }

    log_info("Starting application");
    if (uv_run(uv_default_loop(), UV_RUN_DEFAULT) != 0) {
        return TS_ERR_EVENT_LOOP_START_FAILED;
    }

    return TS_ERR_SUCCESS;
}

void
ts_app_destroy(void)
{
    ts_tcp_listener_stop(listener);
    free(listener);
}
