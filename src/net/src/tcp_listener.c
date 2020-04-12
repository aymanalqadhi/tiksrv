#include "net/tcp_listener.h"
#include "net/tcp_listener_callbacks.h"

#include "log/error.h"
#include "log/logger.h"
#include "util/validation.h"

#include <uthash.h>
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
setup_listener_address(struct ts_tcp_listener *l, const struct ts_config *cfg)
{
    int              rc;
    struct addrinfo *addr;
    char             port_buf[0x10];

    snprintf(port_buf, sizeof(port_buf), "%u", l->port);
    if ((rc = getaddrinfo(cfg->ipv6 ? IPV6_LISTEN_ADDR : IPV4_LISTEN_ADDR,
                          port_buf,
                          NULL,
                          &addr)) != 0) {
        log_debug("getaddrinfo: %s", strerror(errno));
        return TS_ERR_LISTENER_BIND_FAILED;
    }

    if ((rc = uv_tcp_bind(&l->socket, addr->ai_addr, 0)) < 0) {
        freeaddrinfo(addr);
        log_debug("uv_tcp_bind: %s", strerror(rc));
        return TS_ERR_LISTENER_BIND_FAILED;
    }

    freeaddrinfo(addr);
    return TS_ERR_SUCCESS;
}

static inline ts_error_t
tcp_listener_init(struct ts_tcp_listener *l, const struct ts_config *cfg)
{
    int rc;

    l->port = cfg->listen_port;

    if ((rc = uv_tcp_init(uv_default_loop(), &l->socket)) < 0) {
        log_debug("uv_tcp_init: %s", uv_strerror(rc));
        return TS_ERR_SOCKET_CREATE_FAILED;
    } else if ((rc = setup_listener_address(l, cfg)) != 0) {
        return rc;
    }

    l->socket.data          = l;
    l->backlog              = cfg->backlog;
    l->is_running           = false;
    l->clients              = NULL;
    l->client_disconnect_cb = &ts_tcp_listener_disconnected_cb;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_listener_create(struct ts_tcp_listener **             l,
                       struct ts_tcp_listener_app_callbacks *app_cbs,
                       const struct ts_config *              cfg)
{
    int                     rc;
    struct ts_tcp_listener *listener;

    CHECK_NULL_PARAMS_2(l, app_cbs);

    if (!(listener = (struct ts_tcp_listener *)malloc(sizeof(*listener)))) {
        return TS_ERR_MEMORY_ALLOC_FAILED;
    } else if ((rc = tcp_listener_init(listener, cfg)) != 0) {
        free(listener);
        return rc;
    }

    listener->app_cbs = *app_cbs;
    *l                = listener;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_listener_start(struct ts_tcp_listener *l)
{
    int rc;

    CHECK_NULL_PARAMS_1(l);

    if (l->is_running) {
        return TS_ERR_LISTENER_ALREADY_STARTED;
    } else if ((rc = uv_listen((uv_stream_t *)&l->socket,
                               l->backlog,
                               &ts_tcp_listener_accepted_cb)) < 0) {
        log_debug("uv_listen: %s", uv_strerror(rc));
        return TS_ERR_LISTENER_BIND_FAILED;
    }

    l->is_running = true;
    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_listener_stop(struct ts_tcp_listener *l)
{
    int rc;

    CHECK_NULL_PARAMS_1(l);

    if ((rc = uv_tcp_close_reset(&l->socket, &ts_tcp_listener_stop_cb)) < 0) {
        log_debug("uv_tcp_close_reset: %s", uv_strerror(rc));
        return TS_ERR_LISTENER_STOP_FAILED;
    }

    l->is_running = false;
    return TS_ERR_SUCCESS;
}

void
ts_tcp_listener_free(struct ts_tcp_listener *l)
{
    int rc;

    if (l->is_running && (rc = ts_tcp_listener_stop(l)) != 0) {
        log_error("Could not stop listener: %s", ts_strerror(rc));
    }

    free(l);
}
