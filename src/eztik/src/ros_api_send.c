#include "eztik/routeros/api.h"
#include "eztik/routeros/api_sentence.h"

#include "log/error.h"
#include "log/logger.h"
#include "util/validation.h"

#include <glib.h>
#include <uv.h>

#include <assert.h>

struct api_write_context
{
    struct ros_api *         api;
    struct ros_api_sentence *sentence;
    ros_api_send_cb          cb;
    void *                   data;

    uv_buf_t *  buf;
    GByteArray *sbuf;
};

void
api_send_write_cb(uv_write_t *req, int status)
{
    struct api_write_context *ctx = (struct api_write_context *)req->data;

    if (status < 0) {
        log_error("uv_write: %s", uv_strerror(status));
        (*ctx->cb)(false, ctx->sentence, ctx->data);
    } else {
        (*ctx->cb)(true, ctx->sentence, ctx->data);
    }

    g_free(req);
    g_free(ctx->buf);
    g_byte_array_free(ctx->sbuf, TRUE);
    g_free(ctx);
}

ts_error_t
ros_api_send(struct ros_api *         api,
             struct ros_api_sentence *sentence,
             ros_api_send_cb          cb,
             void *                   data)
{
    int         rc;
    uv_write_t *req;
    uv_buf_t *  buf;
    GByteArray *sbuf;

    struct api_write_context *ctx;

    CHECK_NULL_PARAMS_3(api, sentence, cb);
    assert(api->state >= EZTIK_ROS_API_STATE_CONNECTED);

    sbuf      = ros_api_sentence_flatten(sentence);
    buf       = g_new(uv_buf_t, 1);
    buf->len  = sbuf->len;
    buf->base = sbuf->data;

    ctx           = g_new(struct api_write_context, 1);
    ctx->api      = api;
    ctx->sentence = sentence;
    ctx->cb       = cb;
    ctx->data     = data;
    ctx->buf      = buf;
    ctx->sbuf     = sbuf;

    req       = g_new(uv_write_t, 1);
    req->data = ctx;

    if ((rc = uv_write(
             req, (uv_stream_t *)&api->socket, buf, 1, api_send_write_cb))) {
        log_error("uv_write: %s", uv_strerror(rc));
        g_free(req);
        g_free(buf);
        g_byte_array_free(sbuf, TRUE);
        g_free(ctx);
        return TS_ERR_SOCKET_WRITE_FAILED;
    }

    return TS_ERR_SUCCESS;
}
