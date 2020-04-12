#include "app.h"

#include "config.h"
#include "error.h"
#include "log.h"

#include "net/tcp_listener.h"

#include <stdlib.h>

static struct ts_tcp_listener *listener;

static ts_error_t
app_init(const struct ts_config *cfg)
{
    int rc;

    log_info("Creating a TCP listener on port %u...", cfg->listen_port);
    if ((rc = ts_tcp_listener_create(&listener, cfg)) != 0) {
        return rc;
    }

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_app_run(const struct ts_config *cfg)
{
    int rc;

    log_info("Initializing app");
    if ((rc = app_init(cfg)) != 0) {
        return rc;
    }

    log_info("Starting listener");
    if ((rc = ts_tcp_listener_start(listener)) != 0) {
        return rc;
    }

    log_info("Starting application");
    if (uv_run(uv_default_loop(), UV_RUN_DEFAULT) != 0) {
        return TS_ERR_EVENT_LOOP_START_FAILED;
    }

    return TS_ERR_SUCCESS;
}

void
ts_app_destroy(void)
{
    ts_tcp_listener_stop(listener);
    free(listener);
}
