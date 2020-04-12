#include "net/memory.h"
#include "net/message.h"
#include "net/read_state_machine.h"
#include "net/tcp_client.h"
#include "net/tcp_client_callbacks.h"
#include "net/tcp_listener.h"
#include "net/address.h"

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
    } else {
        (*client->read_sm.state)(client, nread);
    }
}
