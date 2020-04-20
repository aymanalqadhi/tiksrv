#include "impl/tcp_listener_callbacks.h"
#include "net/tcp_listener.h"

#include "log/error.h"
#include "log/logger.h"
#include "util/validation.h"

#include <glib.h>
#include <uv.h>

#include <netdb.h>
#include <netinet/in.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define IPV6_LISTEN_ADDR "::"
#define IPV4_LISTEN_ADDR "0.0.0.0"

static inline ts_error_t
setup_listener_address(struct ts_tcp_listener *listener,
                       const struct ts_config *cfg)
{
    int              rc;
    struct addrinfo *addr;
    char             port_buf[0x10];

    snprintf(port_buf, sizeof(port_buf), "%u", listener->port);
    if ((rc = getaddrinfo(cfg->ipv6 ? IPV6_LISTEN_ADDR : IPV4_LISTEN_ADDR,
                          port_buf,
                          NULL,
                          &addr)) != 0) {
        log_debug("getaddrinfo: %s", strerror(errno));
        return TS_ERR_LISTENER_BIND_FAILED;
    }

    if ((rc = uv_tcp_bind(&listener->socket, addr->ai_addr, 0)) < 0) {
        freeaddrinfo(addr);
        log_debug("uv_tcp_bind: %s", strerror(rc));
        return TS_ERR_LISTENER_BIND_FAILED;
    }

    freeaddrinfo(addr);
    return TS_ERR_SUCCESS;
}

static inline ts_error_t
tcp_listener_init(struct ts_tcp_listener *listener, const struct ts_config *cfg)
{
    int rc;

    listener->port = cfg->listen_port;

    if ((rc = uv_tcp_init(uv_default_loop(), &listener->socket)) < 0) {
        log_debug("uv_tcp_init: %s", uv_strerror(rc));
        return TS_ERR_SOCKET_CREATE_FAILED;
    }

    if ((rc = setup_listener_address(listener, cfg)) != 0) {
        return rc;
    }

    listener->socket.data          = listener;
    listener->backlog              = cfg->backlog;
    listener->is_running           = false;
    listener->clients              = NULL;
    listener->client_disconnect_cb = &ts_tcp_listener_disconnected_cb;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_listener_create(struct ts_tcp_listener **             outlistener,
                       struct ts_tcp_listener_app_callbacks *app_cbs,
                       const struct ts_config *              cfg)
{
    int                     rc;
    struct ts_tcp_listener *listener;

    CHECK_NULL_PARAMS_2(outlistener, app_cbs);

    listener = (struct ts_tcp_listener *)g_new(struct ts_tcp_listener, 1);

    if ((rc = tcp_listener_init(listener, cfg)) != 0) {
        g_free(listener);
        return rc;
    }

    listener->app_cbs = *app_cbs;
    *outlistener      = listener;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_listener_start(struct ts_tcp_listener *l)
{
    int rc;

    CHECK_NULL_PARAMS_1(l);

    if (l->is_running) {
        return TS_ERR_LISTENER_ALREADY_STARTED;
    }

    if ((rc = uv_listen((uv_stream_t *)&l->socket,
                        l->backlog,
                        &ts_tcp_listener_accepted_cb)) < 0) {
        log_debug("uv_listen: %s", uv_strerror(rc));
        return TS_ERR_LISTENER_BIND_FAILED;
    }

    l->is_running = true;
    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_listener_stop(struct ts_tcp_listener *listener)
{
    int rc;

    CHECK_NULL_PARAMS_1(listener);

    uv_close((uv_handle_t *)&listener->socket, &ts_tcp_listener_stop_cb);
    listener->is_running = false;

    return TS_ERR_SUCCESS;
}

void
ts_tcp_listener_free(struct ts_tcp_listener *listener)
{
    int rc;

    g_hash_table_destroy(listener->clients);
    if (listener->is_running && (rc = ts_tcp_listener_stop(listener)) != 0) {
        log_error("Could not stop listener: %s", ts_strerror(rc));
    }

    g_free(listener);
}
