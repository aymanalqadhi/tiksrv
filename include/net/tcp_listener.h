#ifndef TIKSRV_NET_TCP_LISTNER_H
#define TIKSRV_NET_TCP_LISTNER_H

#include "config.h"
#include "net/tcp_client.h"

#include <stddef.h>
#include <uv.h>

#include <stdbool.h>

typedef void (*tcp_client_disconnect_cb)(uv_stream_t *);

typedef void (*tcp_client_connected_cb)(struct ts_tcp_client *);
typedef void (*tcp_client_disconnected_cb)(struct ts_tcp_client *);
typedef void (*tcp_client_requested_cb)(struct ts_tcp_client *);

struct ts_tcp_listener
{
    uv_tcp_t listener_handle;

    uint16_t                 port;
    uint16_t                 backlog;
    bool                     is_running;
    tcp_client_disconnect_cb disconnect_cb;

    tcp_client_connected_cb on_connection_cb;
    tcp_client_disconnected_cb on_disconnection_cb;
    tcp_client_requested_cb on_request_cb;

    struct ts_tcp_client *clients;
};

ts_error_t
ts_tcp_listener_create(struct ts_tcp_listener **l, const struct ts_config *cfg);

ts_error_t
ts_tcp_listener_start(struct ts_tcp_listener *l);

ts_error_t
ts_tcp_listener_stop(struct ts_tcp_listener *l);

void
ts_tcp_listener_free(struct ts_tcp_listener *l);

#endif
