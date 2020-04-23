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
#include "services/container.h"

#include <glib.h>

#include <stdlib.h>

struct ts_app app;

static void
export_command(const struct ts_command *cmd)
{
    struct ts_command *tmp;

    if (!cmd) {
        log_warn("A request to export a NULL command");
        return;
    }

    g_hash_table_insert(
        app.commands,
        g_memdup((gconstpointer)&cmd->command, sizeof(cmd->command)),
        g_memdup((gconstpointer)cmd, sizeof(*cmd)));
}

static void
free_plugin(gpointer plug)
{
    ts_plugin_unload(plug);
    free(plug);
}

static ts_error_t
on_plugin_load(struct ts_plugin *plug)
{
    int rc;

    if (app.plugins &&
        g_hash_table_contains(app.plugins, (gpointer)plug->name)) {
        log_warn("Duplicate plugin names %s", plug->name);
        return TS_ERR_SUCCESS;
    }

    if (plug->commads_export_func) {
        if ((rc = (*plug->commads_export_func)(&export_command)) != 0) {
            log_error("Could not export commands for plugin `%s'", plug->name);
            ts_plugin_unload(plug);
            return TS_ERR_LOAD_PLUGIN_FAILED;
        }
    }

    g_hash_table_insert(app.plugins,
                        g_strdup(plug->name),
                        (gpointer)g_memdup(plug, sizeof(*plug)));

    return TS_ERR_SUCCESS;
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
    app.services = ts_services_container_new();
    app.commands =
        g_hash_table_new_full(g_int_hash, g_int_equal, &g_free, &g_free);
    app.plugins =
        g_hash_table_new_full(g_str_hash, g_str_equal, &g_free, &free_plugin);

    log_info("Parsing configuration file");
    if ((rc = ts_config_parse_config_file(cfg))) {
        log_warn("Could not parse configuration file");
    }

    log_info("Initializing services");
    if ((rc = ts_services_container_init(app.services)) != 0) {
        return rc;
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
                    "plugins", &on_plugin_load, cfg, app.services)) != 0) {
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
    g_hash_table_destroy(app.commands);
    g_hash_table_destroy(app.plugins);
    ts_services_container_free(app.services);
}

struct ts_command *
ts_app_get_command(uint32_t id)
{
    return (struct ts_command *)g_hash_table_lookup(app.commands,
                                                    (gconstpointer)&id);
}
