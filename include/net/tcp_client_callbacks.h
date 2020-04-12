#ifndef TIKSRV_NET_TCP_CLIENT_CALLBACKS_H
#define TIKSRV_NET_TCP_CLIENT_CALLBACKS_H

#include <stddef.h>
#include <uv.h>

void
ts_tcp_client_header_read_cb(uv_stream_t *   s,
                             ssize_t         nread,
                             const uv_buf_t *buf);
void
ts_tcp_client_body_read_cb(uv_stream_t *s, ssize_t nread, const uv_buf_t *buf);

void
ts_tcp_client_disconnect_cb(uv_handle_t *h);

#endif
