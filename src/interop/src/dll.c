#include "interop/dll.h"

#include "log/error.h"
#include "log/logger.h"
#include "util/validation.h"

static const char *unknown_plugin_author  = "Unknown author";
static const char *unknown_plugin_version = "Unknow version";

static inline void *
load_symbole(struct ts_plugin *plug, const char *sym)
{
    int   rc;
    void *ptr;

    if ((rc = uv_dlsym(&plug->handle, sym, &ptr)) < 0) {
        log_debug("uv_dlsym: %s", uv_dlerror(&plug->handle));
        return NULL;
    }

    return ptr;
}

ts_error_t
ts_fill_plugins_symoles(struct ts_plugin *plug)
{
    void *ptr;

    CHECK_NULL_PARAMS_1(plug);

    if (!(ptr = load_symbole(plug, TS_PLUGIN_NAME_SYM))) {
        return TS_ERR_LOAD_PLUGIN_FAILED;
    }

    plug->name = *(const char **)ptr;

#define LOAD_OPTIONAL_SYM(to, sym, type, def)                                  \
    if ((ptr = load_symbole(plug, sym))) {                                     \
        to = (type)ptr;                                                        \
    } else {                                                                   \
        to = def;                                                              \
    }

    LOAD_OPTIONAL_SYM(plug->author,
                      TS_PLUGIN_AUTHOR_SYM,
                      const char *,
                      unknown_plugin_author);

    LOAD_OPTIONAL_SYM(plug->version,
                      TS_PLUGIN_VERSION_SYM,
                      const char *,
                      unknown_plugin_version);

    LOAD_OPTIONAL_SYM(plug->init_func,
                      TS_PLUGIN_INIT_FUNCTION_SYM,
                      ts_plugin_init_func_t,
                      NULL);

    LOAD_OPTIONAL_SYM(plug->free_func,
                      TS_PLUGIN_FREE_FUNCTION_SYM,
                      ts_plugin_free_func_t,
                      NULL);

    LOAD_OPTIONAL_SYM(plug->commads_export_func,
                      TS_PLUGIN_COMMANDS_EXPORT_FUNCTION_SYM,
                      ts_plugin_commands_export_func_t,
                      NULL);

    return TS_ERR_SUCCESS;

#undef LOAD_OPTIONAL_SYM
}
