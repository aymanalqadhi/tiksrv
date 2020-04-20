#include "commands/command.h"
#include "config/config.h"
#include "log/error.h"
#include "log/logger.h"
#include "services/container.h"

#include <stdio.h>
#include <stdlib.h>

const char *ts_plugin_name    = "eztik";
const char *ts_plugin_author  = "Ayman Al-Qadhi";
const char *ts_plugin_version = "0.1 ALPHA";

ts_error_t
ts_plugin_init(const struct ts_config *cfg, struct ts_services_container *svcs)
{
    return TS_ERR_SUCCESS;
}

void
ts_plugin_free(void)
{
}

ts_error_t
ts_plugin_export_commands(ts_command_export_func_t export_func)
{
    return TS_ERR_SUCCESS;
}
