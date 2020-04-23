#include "eztik/routeros/api.h"

#include <glib.h>
#include <uv.h>

#include <assert.h>

struct ros_api_free_context
{
    struct ros_api *api;
    ros_api_free_cb cb;
    void *          data;
};

static void
ros_api_free_close_cb(uv_handle_t *h)
{
    struct ros_api_free_context *ctx = (struct ros_api_free_context *)h->data;

    g_free(ctx->api);
    (*ctx->cb)(ctx->data);
    g_free(ctx);
}

struct ros_api *
ros_api_new(void)
{
    int             rc;
    struct ros_api *ret;

    ret = g_new(struct ros_api, 1);

    if ((rc = uv_tcp_init(uv_default_loop(), &ret->socket)) < 0) {
        return NULL;
    }

    ret->state = EZTIK_ROS_API_STATE_DISCONNECTED;
    return ret;
}

void
ros_api_free(struct ros_api *api, ros_api_free_cb cb, void *data)
{
    struct ros_api_free_context *ctx;

    assert(api != NULL && cb != NULL);

    if (api->state != EZTIK_ROS_API_STATE_DISCONNECTED) {
        if (api->state != EZTIK_ROS_API_STATE_CLOSING) {
            ctx              = g_new(struct ros_api_free_context, 1);
            ctx->api         = api;
            ctx->cb          = cb;
            ctx->data        = data;
            api->state       = EZTIK_ROS_API_STATE_CLOSING;
            api->socket.data = ctx;
            uv_close((uv_handle_t *)&api->socket, ros_api_free_close_cb);
        }
    } else {
        g_free(api);
        (*cb)(data);
    }
}
