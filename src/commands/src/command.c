#include "commands/command.h"

#include "log/error.h"
#include "services/container.h"

#include <stdint.h>

extern struct ts_command ts_echo_command;

ts_error_t
ts_commands_export(ts_command_exprt_func_t export_func)
{
    (*export_func)(&ts_echo_command);

    return TS_ERR_SUCCESS;
}
