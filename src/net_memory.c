#include "net/memory.h"
#include "net/message.h"
#include "net/read_context.h"
#include "net/tcp_client.h"

#include <uv.h>

#include <stdlib.h>

void
ts_read_buffer_alloc_cb(uv_handle_t *handle, size_t s_size, uv_buf_t *buf)
{
    struct ts_tcp_client *client = (struct ts_tcp_client *)handle->data;

    if (client->read_ctx.state == TS_READ_STATE_HEADER) {
        if (!client->read_ctx.buf) {
            client->read_ctx.buf =
                (char *)malloc(TS_MESSAGE_REQUEST_HEADER_SIZE);
            client->read_ctx.nread = 0;
            buf->base              = (char *)client->read_ctx.buf;
            buf->len               = TS_MESSAGE_REQUEST_HEADER_SIZE;
        } else {
            buf->base = client->read_ctx.buf + client->read_ctx.nread;
            buf->len  = TS_MESSAGE_REQUEST_HEADER_SIZE - client->read_ctx.nread;
        }
    } else {
        if (!client->read_ctx.buf) {
            client->read_ctx.buf =
                (char *)malloc(client->read_ctx.header.body_length);
            client->read_ctx.nread = 0;
            buf->base              = (char *)client->read_ctx.buf;
            buf->len               = client->read_ctx.header.body_length;
        } else {
            buf->base = client->read_ctx.buf + client->read_ctx.nread;
            buf->len =
                client->read_ctx.header.body_length - client->read_ctx.nread;
        }
    }
}
