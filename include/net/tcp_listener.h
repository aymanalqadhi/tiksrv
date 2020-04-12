#ifndef TIKSRV_NET_TCP_LISTNER_H
#define TIKSRV_NET_TCP_LISTNER_H

#include "config.h"
#include "net/message.h"
#include "net/tcp_client.h"

#include <stddef.h>
#include <uv.h>

#include <stdbool.h>

struct ts_tcp_listener_app_callbacks
{
    void (*on_connection_cb)(struct ts_tcp_client *);
    void (*on_disconnection_cb)(struct ts_tcp_client *);
    void (*on_request_cb)(struct ts_tcp_client *,
                          const struct ts_request_message *);
};

struct ts_tcp_listener
{
    uv_tcp_t socket;

    uint16_t port;
    uint16_t backlog;
    bool     is_running;
    void (*client_disconnect_cb)(uv_stream_t *);

    struct ts_tcp_listener_app_callbacks app_cbs;
    struct ts_tcp_client *               clients;
};

ts_error_t
ts_tcp_listener_create(struct ts_tcp_listener **             l,
                       struct ts_tcp_listener_app_callbacks *app_cbs,
                       const struct ts_config *              cfg);

ts_error_t
ts_tcp_listener_start(struct ts_tcp_listener *l);

ts_error_t
ts_tcp_listener_stop(struct ts_tcp_listener *l);

void
ts_tcp_listener_free(struct ts_tcp_listener *l);

#endif
