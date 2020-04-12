#ifndef TIKSRV_NET_TCP_LISTENRER_CALLBACKS_H
#define TIKSRV_NET_TCP_LISTENRER_CALLBACKS_H

#include "net/tcp_client.h"

#include <uv.h>

void
ts_tcp_listener_stop_cb(uv_handle_t *handle);

void
ts_tcp_listener_accepted_cb(uv_stream_t *stream, int status);

void
ts_tcp_listener_disconnected_cb(uv_stream_t *stream);

#endif
