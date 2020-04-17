#ifndef TIKSRV_INTEROP_PLUGIN_H
#define TIKSRV_INTEROP_PLUGIN_H

#include "commands/command.h"
#include "services/container.h"

#include "log/error.h"

#include "uthash.h"
#include "uv.h"

#include <stddef.h>

#define TS_PLUGIN_NAME_SYM                     "ts_plugin_name"
#define TS_PLUGIN_AUTHOR_SYM                   "ts_plugin_author"
#define TS_PLUGIN_VERSION_SYM                  "ts_plugin_version"
#define TS_PLUGIN_COMMANDS_EXPORT_FUNCTION_SYM "ts_plugin_commands_export"
#define TS_PLUGIN_INIT_FUNCTION_SYM            "ts_plugin_init"
#define TS_PLUGIN_FREE_FUNCTION_SYM            "ts_plugin_free"

struct ts_plugin;

typedef ts_error_t (*ts_plugin_commands_export_func_t)(ts_command_exprt_func_t);
typedef ts_error_t (*ts_plugin_init_func_t)(struct ts_services_container *);
typedef void (*ts_plugin_free_func_t)(void);

typedef ts_error_t (*ts_plugin_load_cb)(struct ts_plugin *);

struct ts_plugin
{
    const char *name;
    const char *author;
    const char *version;

    ts_plugin_init_func_t            init_func;
    ts_plugin_free_func_t            free_func;
    ts_plugin_commands_export_func_t commads_export_func;

    uv_lib_t       handle;
    UT_hash_handle hh;
};

ts_error_t
ts_plugin_load(struct ts_plugin *plug, const char *filename);

void
ts_plugin_unload(struct ts_plugin *plug);

ts_error_t
ts_plugin_load_all(const char *dirname, ts_plugin_load_cb cb);

#endif
