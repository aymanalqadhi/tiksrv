#include "eztik/routeros/api.h"
#include "eztik/session.h"

#include "log/error.h"
#include "log/logger.h"

#include <glib.h>
#include <uv.h>

#include <assert.h>
#include <stdint.h>
#include <string.h>

struct session_api_open_context
{
    struct eztik_session *session;
    eztik_session_new_cb  cb;
    void *                data;
};

static void
session_api_close_cb(void *data)
{
    struct session_api_open_context *ctx =
        (struct session_api_open_context *)data;

    (*ctx->cb)(NULL, ctx->data);

    g_free(ctx->session);
    g_free(ctx);
}

static void
session_api_open_cb(bool success, void *data)
{
    struct session_api_open_context *ctx =
        (struct session_api_open_context *)data;

    if (!success) {
        ros_api_free(ctx->session->api, &session_api_close_cb, (void *)ctx);
    } else {
        (*ctx->cb)(ctx->session, ctx->data);
        g_free(ctx);
    }
}

void
eztik_session_new(uint32_t id, eztik_session_new_cb cb, void *data)
{
    int                              rc;
    struct eztik_session *           ret;
    struct session_api_open_context *ctx;

    assert(cb != NULL);

    ret = g_new(struct eztik_session, 1);

    if (!(ret->api = ros_api_new())) {
        g_free(ret);
        (*cb)(NULL, data);
        return;
    }

    ret->id = id;

    ctx          = g_new(struct session_api_open_context, 1);
    ctx->session = ret;
    ctx->cb      = cb;
    ctx->data    = data;

    if ((rc = ros_api_open(ret->api, &session_api_open_cb, (void *)ctx)) != 0) {
        log_error("ros_api_open: %s", ts_strerror(rc));
        g_free(ret);
        g_free(ctx);
        (*cb)(NULL, data);
    }
}

void
eztik_session_free(struct eztik_session *session)
{
    assert(session != NULL);
    ros_api_free(session->api, &g_free, session);
}
