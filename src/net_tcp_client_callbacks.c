#include "net/tcp_client.h"
#include "net/tcp_client_callbacks.h"
#include "net/tcp_listener.h"

#include "net/address.h"

#include "log.h"

#include <uv.h>

void
ts_tcp_client_header_read_cb(uv_stream_t *s, ssize_t nread, const uv_buf_t *buf)
{
    struct ts_tcp_client *client = (struct ts_tcp_client *)s->data;

    if (nread != TS_MESSAGE_REQUEST_HEADER_SIZE) {
        free(buf->base);
        uv_close((uv_handle_t *)s, NULL);
        (*client->listener->disconnection_cb)(s);
        return;
    }
}

void
ts_tcp_client_body_read_cb(uv_stream_t *s, ssize_t nread, const uv_buf_t *buf)
{}
