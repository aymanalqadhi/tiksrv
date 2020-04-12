#include "net/memory.h"
#include "net/message.h"
#include "net/read_context.h"
#include "net/tcp_client.h"
#include "net/tcp_client_callbacks.h"
#include "net/tcp_listener.h"

#include "net/address.h"

#include "log.h"

#include <uv.h>

static inline void
tcp_client_start_read(struct ts_tcp_client *client)
{
    int rc;

    if ((rc = uv_read_start((uv_stream_t *)&client->socket,
                            &ts_read_buffer_alloc_cb,
                            &ts_tcp_client_read_cb)) < 0) {
        log_error("uv_read_start: %s", uv_strerror(rc));

        if (client->read_ctx.buf) {
            free(client->read_ctx.buf);
        }

        uv_close((uv_handle_t *)&client->socket, NULL);
        (*client->listener->disconnect_cb)((uv_stream_t *)&client->socket);
        return;
    }
}

static inline void
tcp_client_read_header_done(struct ts_tcp_client *client)
{
    int rc;

    if ((rc = ts_decode_request_header(&client->read_ctx.header,
                                       client->read_ctx.buf,
                                       client->read_ctx.nread)) != 0) {
        log_error("ts_decode_request_header: %s", ts_strerror(rc));
        return;
    }

    free(client->read_ctx.buf);
    client->read_ctx.buf   = NULL;
    client->read_ctx.nread = 0;
    client->read_ctx.state = TS_READ_STATE_BODY;

    log_debug("Got Request Header:");
    log_debug("[+] Command: %#08X", client->read_ctx.header.command);
    log_debug("[+] Flags: %#08X", client->read_ctx.header.flags);
    log_debug("[+] Sequence Number: %#08X", client->read_ctx.header.seq_number);
    log_debug("[+] Body Length: %#08X", client->read_ctx.header.body_length);

    tcp_client_start_read(client);
}

static inline void
tcp_client_read_body_done(struct ts_tcp_client *client)
{
    if (client->listener->on_request_cb) {
        (*client->listener->on_request_cb)(client);
    }

    free(client->read_ctx.buf);
    client->read_ctx.buf   = NULL;
    client->read_ctx.nread = 0;
    client->read_ctx.state = TS_READ_STATE_HEADER;
}

static inline void
tcp_client_read_header(struct ts_tcp_client *client, ssize_t nread)
{
    client->read_ctx.nread += nread;
    if (client->read_ctx.nread + nread >= TS_MESSAGE_REQUEST_HEADER_SIZE) {
        tcp_client_read_header_done(client);
    }
}

static inline void
tcp_client_read_body(struct ts_tcp_client *client, ssize_t nread)
{
    client->read_ctx.nread += nread;
    if (client->read_ctx.nread + nread >= client->read_ctx.header.body_length) {
        tcp_client_read_body_done(client);
    }
}

void
ts_tcp_client_read_cb(uv_stream_t *s, ssize_t nread, const uv_buf_t *buf)
{
    struct ts_tcp_client *client = (struct ts_tcp_client *)s->data;

    if (nread < 0 || !buf->base) {
        uv_close((uv_handle_t *)s, NULL);
        (*client->listener->disconnect_cb)(s);
        return;
    } else if (nread == 0) {
        tcp_client_start_read(client);
    } else {
        if (client->read_ctx.state == TS_READ_STATE_HEADER) {
            tcp_client_read_header(client, nread);
        } else {
            tcp_client_read_body(client, nread);
        }
    }
}
