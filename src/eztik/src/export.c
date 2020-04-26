#include "eztik/commands.h"
#include "eztik/eztik.h"

#include "commands/command.h"
#include "config/config.h"
#include "log/error.h"
#include "services/container.h"

#include <stdio.h>
#include <stdlib.h>

const char *ts_plugin_name    = "eztik";
const char *ts_plugin_author  = "Ayman Al-Qadhi";
const char *ts_plugin_version = "0.1 ALPHA";

ts_error_t
ts_plugin_init(const struct ts_config *cfg, struct ts_services_container *svcs)
{
    return eztik_init(cfg, svcs);
}

void
ts_plugin_free(void)
{
    eztik_destroy();
}

ts_error_t
ts_plugin_commands_export(ts_command_export_func_t export_func)
{
    (*export_func)(EZTIK_COMMANDS_TYPE, &eztik_ros_api_open_command);
    (*export_func)(EZTIK_COMMANDS_TYPE, &eztik_ros_api_close_command);
    (*export_func)(EZTIK_COMMANDS_TYPE, &eztik_ros_api_send_raw_command);
    (*export_func)(EZTIK_COMMANDS_TYPE, &eztik_ros_version_command);

    return TS_ERR_SUCCESS;
}
