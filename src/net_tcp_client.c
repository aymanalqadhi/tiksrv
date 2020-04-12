#include "net/tcp_client.h"
#include "net/tcp_client_callbacks.h"
#include "net/tcp_listener.h"

#include "error.h"
#include "log.h"
#include "net/memory.h"
#include "utils/validation.h"

#include <stdint.h>
#include <stdlib.h>

static inline ts_error_t
tcp_client_init(struct ts_tcp_client *client)
{
    int             rc;
    static uint32_t currnet_id = 0;

    if ((rc = uv_tcp_init(uv_default_loop(), &client->socket)) < 0) {
        log_debug("uv_tcp_init: %s", uv_strerror(rc));
        return TS_ERR_SOCKET_CREATE_FAILED;
    }

    client->id              = currnet_id++;
    client->socket.data     = client;
    client->read_sm.state   = &ts_read_header_state;
    client->read_sm.read_cb = &ts_tcp_client_read_cb;
    client->read_sm.buf     = NULL;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_client_create(struct ts_tcp_client **c)
{
    int                   rc;
    struct ts_tcp_client *client;

    CHECK_NULL_PARAMS_1(c);

    if (!(client = (struct ts_tcp_client *)malloc(sizeof(*client)))) {
        return TS_ERR_MEMORY_ALLOC_FAILED;
    } else if ((rc = tcp_client_init(client)) != 0) {
        free(client);
        return rc;
    }

    *c = client;
    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_client_start_read(struct ts_tcp_client *client)
{
    int rc;

    if ((rc = uv_read_start((uv_stream_t *)&client->socket,
                            &ts_read_buffer_alloc_cb,
                            client->read_sm.read_cb)) < 0) {
        log_error("uv_read_start: %s", uv_strerror(rc));

        return rc;
    }

    return TS_ERR_SUCCESS;
}

void
ts_tcp_client_close(struct ts_tcp_client *client)
{
    uv_close((uv_handle_t *)&client->socket, NULL);
    (*client->listener->client_disconnect_cb)((uv_stream_t *)&client->socket);
}

void
ts_tcp_client_free(struct ts_tcp_client *client)
{
    free(client);
    if (client->read_sm.buf) {
        free(client->read_sm.buf);
    }
}
