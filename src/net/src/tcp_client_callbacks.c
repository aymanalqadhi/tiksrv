#include "impl/tcp_client_callbacks.h"

#include "net/address.h"
#include "net/memory.h"
#include "net/message.h"
#include "net/read_state_machine.h"
#include "net/tcp_client.h"
#include "net/tcp_listener.h"

#include "log/logger.h"

#include "uv.h"

void
ts_tcp_client_read_cb(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    int                   rc;
    struct ts_tcp_client *client;

    client = (struct ts_tcp_client *)stream->data;

    if (nread < 0 || !buf->base) {
        ts_tcp_client_close(client);
        return;
    }

    (*client->read_sm.state)(client, nread);
}

void
ts_tcp_client_write_cb(uv_write_t *req, int status)
{
    int                   i;
    struct ts_tcp_client *client;

    client = (struct ts_tcp_client *)req->data;

    ts_response_context_free(&client->response_ctx);
    free(req);

    if (status < 0) {
        log_error("uv_write: %s", uv_strerror(status));
        ts_tcp_client_close(client);
        return;
    }
}

void
ts_tcp_client_close_cb(uv_handle_t *socket)
{
    struct ts_tcp_client *client = (struct ts_tcp_client *)socket->data;
    (*client->listener->client_disconnect_cb)((uv_stream_t *)&client->socket);
}
