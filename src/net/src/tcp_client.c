#include "net/tcp_client.h"
#include "impl/tcp_client_callbacks.h"

#include "log/error.h"
#include "log/logger.h"
#include "net/memory.h"
#include "util/validation.h"

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
    }

    if ((rc = tcp_client_init(client)) != 0) {
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
    if (!uv_is_closing((uv_handle_t *)&client->socket)) {
        uv_close((uv_handle_t *)&client->socket, &ts_tcp_client_close_cb);
    }
}

void
ts_tcp_client_free(struct ts_tcp_client *client)
{
    if (client->read_sm.buf) {
        free(client->read_sm.buf);
    }

    free(client);
}

ts_error_t
ts_tcp_client_respond(struct ts_tcp_client *      client,
                      struct ts_response_message *resp)
{
    int         rc;
    uv_write_t *write_req;

    CHECK_NULL_PARAMS_2(client, resp);

    if (!(write_req = (uv_write_t *)malloc(sizeof(*write_req)))) {
        return TS_ERR_MEMORY_ALLOC_FAILED;
    }

    if ((rc = ts_response_context_init(&client->response_ctx, resp)) != 0) {
        free(write_req);
        return rc;
    }

    write_req->data = client;

    if ((rc = uv_write(write_req,
                       (uv_stream_t *)&client->socket,
                       client->response_ctx.buffers,
                       2,
                       &ts_tcp_client_write_cb)) < 0) {
        log_error("uv_write: %s\n", uv_strerror(rc));
        ts_response_context_free(&client->response_ctx);
        free(write_req);

        return rc;
    }

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_client_respond_with_code(struct ts_tcp_client *client,
                                uint16_t              code,
                                uint32_t              seq_no)
{
    struct ts_response_message resp;
    struct ts_response_header  resp_header;

    resp_header.body_length = 0;
    resp_header.code        = code;
    resp_header.seq_number  = seq_no;
    resp_header.flags       = 0;

    resp.body   = NULL;
    resp.header = &resp_header;

    return ts_tcp_client_respond(client, &resp);
}
