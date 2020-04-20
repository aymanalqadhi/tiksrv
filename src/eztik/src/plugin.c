#include "commands/command.h"
#include "log/error.h"
#include "log/logger.h"
#include "services/container.h"

const char *ts_plugin_name    = "eztik";
const char *ts_plugin_author  = "Ayman Al-Qadhi";
const char *ts_plugin_version = "0.1 ALPHA";

ts_error_t
ts_plugin_init(struct ts_services_container *svcs)
{
    log_norm("Initializng eztik plugin");
    printf("here am i\n");
    return TS_ERR_SUCCESS;
}

void
ts_plugin_free(void)
{
    log_norm("Destroying eztik plugin");
}

ts_error_t
ts_plugin_export_commands(ts_command_export_func_t export_func)
{
    return TS_ERR_SUCCESS;
}
