#include "log/logger.h"
#include "net/memory.h"
#include "net/message.h"
#include "net/read_state_machine.h"
#include "net/tcp_client.h"

#include "uv.h"

#include <stdlib.h>

void
ts_read_buffer_alloc_cb(uv_handle_t *handle, size_t s_size, uv_buf_t *buf)
{
    struct ts_tcp_client *client = (struct ts_tcp_client *)handle->data;

    size_t to_read;

    if (client->read_sm.state == &ts_read_header_state) {
        to_read = TS_MESSAGE_REQUEST_HEADER_SIZE;
    } else {
        if (client->read_sm.header.body_length > TS_MAX_BODY_BUFFER_SIZE) {
            log_warn(
                "Client #%u has requested a large buffer (%u bytes, max: %u)",
                client->id,
                client->read_sm.header.body_length,
                TS_MAX_BODY_BUFFER_SIZE);
            buf->base = NULL;
            buf->len  = 0;
            return;
        }
        to_read = client->read_sm.header.body_length;
    }

    if (!client->read_sm.buf) {
        client->read_sm.buf   = malloc(to_read);
        client->read_sm.nread = 0;
    }

    buf->base = client->read_sm.buf + client->read_sm.nread;
    buf->len  = to_read - client->read_sm.nread;
}
