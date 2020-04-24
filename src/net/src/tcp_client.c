#include "impl/tcp_client_callbacks.h"
#include "net/tcp_client.h"

#include "log/error.h"
#include "log/logger.h"
#include "net/memory.h"
#include "net/message.h"
#include "util/validation.h"

#include <glib.h>

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

static void
response_deallocator(gpointer ptr)
{
    struct ts_response_message *resp = (struct ts_response_message *)ptr;

    if (resp->body) {
        g_free((gpointer)resp->body);
    }

    g_free(resp);
}

static inline ts_error_t
tcp_client_init(struct ts_tcp_client *client)
{
    int             rc;
    static uint32_t currnet_id = 0;

    client->write_queue = g_queue_new();

    if ((rc = uv_tcp_init(uv_default_loop(), &client->socket)) < 0) {
        log_debug("uv_tcp_init: %s", uv_strerror(rc));
        return TS_ERR_SOCKET_CREATE_FAILED;
    }

    client->id              = currnet_id++;
    client->ref_count       = 1;
    client->socket.data     = client;
    client->read_sm.state   = &ts_read_header_state;
    client->read_sm.read_cb = &ts_tcp_client_read_cb;
    client->read_sm.buf     = NULL;
    client->state           = TS_TCP_CLIENT_STATE_DISCONNECTED;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_client_create(struct ts_tcp_client **c)
{
    int                   rc;
    struct ts_tcp_client *client;

    CHECK_NULL_PARAMS_1(c);

    client = (struct ts_tcp_client *)g_new(struct ts_tcp_client, 1);

    if ((rc = tcp_client_init(client)) != 0) {
        g_free((gpointer)client);
        return rc;
    }

    *c = client;
    return TS_ERR_SUCCESS;
}

void
ts_tcp_client_ref(struct ts_tcp_client *client)
{
    ++client->ref_count;
}

void
ts_tcp_client_unref(struct ts_tcp_client *client)
{
    assert(client != NULL);
    assert(client->ref_count > 0);

    if (--client->ref_count == 0) {
        ts_tcp_client_free(client);
    }
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
        client->state = TS_TCP_CLIENT_STATE_DISCONNECTING;
        uv_close((uv_handle_t *)&client->socket, &ts_tcp_client_close_cb);
    }
}

void
ts_tcp_client_free(struct ts_tcp_client *client)
{
    if (client->read_sm.buf) {
        g_free(client->read_sm.buf);
    }

    g_queue_free_full(client->write_queue, &response_deallocator);
    g_free(client);
}

ts_error_t
ts_tcp_client_enqueue_response(struct ts_tcp_client *      client,
                               struct ts_response_message *resp)
{
    CHECK_NULL_PARAMS_1(client);

    if (client->state != TS_TCP_CLIENT_STATE_CONNECTED) {
        return TS_ERR_SOCKET_INVALID_STATE;
    }

    g_queue_push_tail(client->write_queue,
                      (gpointer)g_memdup(resp, sizeof(*resp)));

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_client_send_equeued(struct ts_tcp_client *client)
{
    int         rc;
    uv_write_t *write_req;

    struct ts_response_message *resp;
    struct ts_write_context *   ctx;

    if (client->state != TS_TCP_CLIENT_STATE_CONNECTED) {
        return TS_ERR_SOCKET_INVALID_STATE;
    }

    if (g_queue_is_empty(client->write_queue)) {
        return TS_ERR_SUCCESS;
    }

    resp = (struct ts_response_message *)g_queue_pop_head(client->write_queue);
    write_req = (uv_write_t *)g_new(uv_write_t, 1);

    if ((rc = ts_write_context_create(&ctx, client, resp)) != 0) {
        response_deallocator(resp);
        g_free(write_req);
        return rc;
    }

    write_req->data = ctx;

    if ((rc = uv_write(write_req,
                       (uv_stream_t *)&client->socket,
                       ctx->buffers,
                       ctx->has_body ? 2 : 1,
                       &ts_tcp_client_write_cb)) < 0) {
        log_error("uv_write: %s", uv_strerror(rc));
        ts_write_context_free(ctx);
        free(write_req);

        return rc;
    }

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_client_respond(struct ts_tcp_client *      client,
                      struct ts_response_message *resp)
{
    int rc;

    CHECK_NULL_PARAMS_2(client, resp);

    if ((rc = ts_tcp_client_enqueue_response(client, resp)) != 0) {
        return rc;
    }

    return ts_tcp_client_send_equeued(client);
}

ts_error_t
ts_tcp_client_respond_with_code(struct ts_tcp_client *client,
                                uint16_t              code,
                                uint32_t              seq_no)
{
    struct ts_response_message resp;
    struct ts_response_header  resp_header;

    resp_header.type        = TS_RESPONSE_TYPE_SYSTEM;
    resp_header.body_length = 0;
    resp_header.code        = code;
    resp_header.seq_number  = seq_no;
    resp_header.flags       = 0;

    resp.body   = NULL;
    resp.header = &resp_header;

    return ts_tcp_client_respond(client, &resp);
}

ts_error_t
ts_write_context_create(struct ts_write_context **  outctx,
                        struct ts_tcp_client *      client,
                        struct ts_response_message *resp)
{
    int                      rc;
    struct ts_write_context *ctx;

    ctx = (struct ts_write_context *)g_new(struct ts_write_context, 1);

    if ((rc = ts_write_context_init(ctx, client, resp)) != 0) {
        return rc;
    }

    *outctx = ctx;
    return TS_ERR_SUCCESS;
}

ts_error_t
ts_write_context_init(struct ts_write_context *   ctx,
                      struct ts_tcp_client *      client,
                      struct ts_response_message *resp)
{
    int rc;

    CHECK_NULL_PARAMS_2(ctx, resp);

    ctx->buffers[0].base = (char *)g_malloc(TS_MESSAGE_RESPONSE_HEADER_SIZE);

    if ((rc = ts_encode_response_header(resp->header,
                                        ctx->buffers[0].base,
                                        TS_MESSAGE_RESPONSE_HEADER_SIZE)) !=
        0) {
        g_free(ctx->buffers[0].base);
        return TS_ERR_RESPONSE_HEADER_ENCODE_FAILED;
    }

    ctx->client         = client;
    ctx->response       = resp;
    ctx->buffers[0].len = TS_MESSAGE_RESPONSE_HEADER_SIZE;

    if (resp->body) {
        ctx->buffers[1].len  = resp->header->body_length;
        ctx->buffers[1].base = (char *)resp->body;
        ctx->has_body        = true;
    } else {
        ctx->has_body = false;
    }

    return TS_ERR_SUCCESS;
}

void
ts_write_context_free(struct ts_write_context *ctx)
{
    g_free(ctx->buffers[0].base);
    response_deallocator(ctx->response);
    g_free(ctx);
}
