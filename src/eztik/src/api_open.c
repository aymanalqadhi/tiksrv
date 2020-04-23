#include "eztik/routeros/api.h"

#include "eztik/eztik.h"

#include "config/config.h"
#include "log/error.h"
#include "log/logger.h"
#include "util/validation.h"

#include <glib.h>
#include <uv.h>

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct api_open_context
{
    struct ros_api *api;
    ros_api_open_cb cb;
    void *          data;
};

static void
api_open_close_cb(uv_handle_t *sock)
{
    struct api_open_context *ctx = (struct api_open_context *)sock->data;

    assert(ctx->api->state == EZTIK_ROS_API_STATE_CONNECTING);

    ctx->api->state = EZTIK_ROS_API_STATE_DISCONNECTED;
    (*ctx->cb)(false, ctx->data);
    g_free(ctx);
}

static void
api_open_connect_cb(uv_connect_t *req, int status)
{
    struct api_open_context *ctx = (struct api_open_context *)req->data;

    assert(ctx->api->state == EZTIK_ROS_API_STATE_CONNECTING);
    g_free(req);

    if (status < 0) {
        log_error("uv_tcp_connect: %s", uv_strerror(status));
        ctx->api->socket.data = (void *)ctx;
        uv_close((uv_handle_t *)&ctx->api->socket, api_open_close_cb);
        return;
    }

    ctx->api->state = EZTIK_ROS_API_STATE_CONNECTED;
    (*ctx->cb)(true, ctx->data);
    g_free(ctx);
}

ts_error_t
ros_api_open(struct ros_api *api, ros_api_open_cb cb, void *data)
{
    int         rc;
    const char *ip;
    uint16_t    port;

    struct api_open_context *ctx;
    struct sockaddr_in       addr;
    uv_connect_t *           conn_req;

    CHECK_NULL_PARAMS_2(api, cb);
    assert(api->state == EZTIK_ROS_API_STATE_DISCONNECTED);

    if (!(ip = EZTIK_CONFIG(EZTIK_CONFIG_ROS_IP, NULL))) {
        log_warn("No RouterOS IP address was found in the configuration file");
        return TS_ERR_CONFIG_REQUIRED_KEY_NOT_FOUND;
    }

    port = EZTIK_CONFIG_UINT16(EZTIK_CONFIG_ROS_API_PORT,
                               EZTIK_CONFIG_DEFAULT_API_PORT);

    if ((rc = uv_ip4_addr(ip, port, &addr)) < 0) {
        log_error("uv_ip4_addr: %s", uv_strerror(rc));
        return TS_ERR_LOAD_ADDRESS_FAILED;
    }

    ctx       = g_new(struct api_open_context, 1);
    ctx->api  = api;
    ctx->cb   = cb;
    ctx->data = data;

    conn_req       = g_new(uv_connect_t, 1);
    conn_req->data = ctx;

    log_debug("Connecting to %s:%u", ip, port);
    api->state = EZTIK_ROS_API_STATE_CONNECTING;

    if ((rc = uv_tcp_connect(conn_req,
                             &ctx->api->socket,
                             (struct sockaddr *)&addr,
                             &api_open_connect_cb)) < 0) {
        log_error("uv_tcp_connect: %s", uv_strerror(rc));
        uv_close((uv_handle_t *)&api->socket, NULL);
        g_free(conn_req);
        g_free(ctx);
        api->state = EZTIK_ROS_API_STATE_DISCONNECTED;
        return TS_ERR_SOCKET_OPEN_FAILED;
    }

    return TS_ERR_SUCCESS;
}
