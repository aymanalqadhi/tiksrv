#include "app/app.h"

#include "impl/callbacks.h"
#include "impl/shared.h"

#include "commands/command.h"
#include "config/config.h"
#include "interop/filesystem.h"
#include "interop/plugin.h"
#include "log/error.h"
#include "log/logger.h"
#include "net/address.h"
#include "net/tcp_client.h"
#include "net/tcp_listener.h"
#include "util/memory.h"

#include "uthash.h"

#include <stdlib.h>

static struct
{
    struct ts_config *      config;
    struct ts_tcp_listener *listener;
    struct ts_plugin *      plugins;
    struct ts_command *     commands;
} app;

static void
export_command(const struct ts_command *cmd)
{
    struct ts_command *tmp;

    if (!cmd) {
        log_warn("A request to export a NULL command");
        return;
    }

    if (!(tmp = (struct ts_command *)ts_memdup(cmd, sizeof(*cmd)))) {
        log_error("Could not add command with command number: %d",
                  cmd->command);
        return;
    }

    HASH_ADD_INT(app.commands, command, tmp);
}

static inline void
destroy_commands(void)
{
    struct ts_command *command, *tmp;

    HASH_ITER(hh, app.commands, command, tmp)
    {
        HASH_DEL(app.commands, command);
        free(command);
    }
}

static ts_error_t
on_plugin_load(struct ts_plugin *plug)
{
    int               rc;
    struct ts_plugin *tmp;

    tmp = NULL;
    HASH_FIND_STR(app.plugins, plug->name, tmp);

    if (tmp) {
        log_warn("Duplicate plugin names %s", tmp->name);
        return TS_ERR_SUCCESS;
    }

    if (plug->commads_export_func) {
        if ((rc = (*plug->commads_export_func)(&export_command)) != 0) {
            log_error("Could not export commands for plugin `%s'", plug->name);
            ts_plugin_unload(plug);
            return TS_ERR_LOAD_PLUGIN_FAILED;
        }
    }

    if (!(tmp = (struct ts_plugin *)ts_memdup(plug, sizeof(*plug)))) {
        log_error("Could not add plugin `%s'", plug->name);
        return TS_ERR_MEMORY_ALLOC_FAILED;
    }

    HASH_ADD_STR(app.plugins, name, tmp);
    return TS_ERR_SUCCESS;
}

static inline void
destroy_plugins(void)
{
    struct ts_plugin *plug, *tmp;

    HASH_ITER(hh, app.plugins, plug, tmp)
    {
        HASH_DEL(app.plugins, plug);
        ts_plugin_unload(plug);
        free(plug);
    }
}

static ts_error_t
app_init(struct ts_config *cfg)
{
    int rc;

    struct ts_tcp_listener_app_callbacks app_cbs = {
        .on_connection_cb    = &ts_on_client_connection,
        .on_disconnection_cb = &ts_on_client_disconnection,
        .on_request_cb       = &ts_on_client_request
    };

    app.config = cfg;

    log_info("Parsing configuration file");
    if ((rc = ts_config_parse_config_file(cfg))) {
        log_warn("Could not parse configuration file");
    }

    log_info("Creating a TCP listener on port %u", cfg->listen_port);
    if ((rc = ts_tcp_listener_create(&app.listener, &app_cbs, cfg)) != 0) {
        return rc;
    }

    log_info("Exporting system commands");
    if ((rc = ts_commands_export(&export_command)) != 0) {
        return rc;
    }

    log_info("Loading plugins");
    if (!ts_is_directory("plugins")) {
        log_debug("No plugins directory detected, skipping plugins loading");
    } else if ((rc = ts_plugin_load_all(
                    "plugins", &on_plugin_load, cfg, NULL)) != 0) {
        return rc;
    }

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_app_run(struct ts_config *cfg)
{
    int rc;

    log_info("Initializing app");
    if ((rc = app_init(cfg)) != 0) {
        return rc;
    }

    log_info("Starting listener");
    if ((rc = ts_tcp_listener_start(app.listener)) != 0) {
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
    log_info("Cleaning up");
    ts_tcp_listener_free(app.listener);

    ts_config_free(app.config);
    destroy_commands();
    destroy_plugins();
}

struct ts_command *
ts_app_get_command(uint32_t id)
{
    struct ts_command *out;
    HASH_FIND_INT(app.commands, &id, out);

    return out;
}
