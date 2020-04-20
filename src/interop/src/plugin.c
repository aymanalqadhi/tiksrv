#include "interop/dll.h"
#include "interop/filesystem.h"
#include "interop/plugin.h"

#include "commands/command.h"
#include "services/container.h"
#include "util/memory.h"
#include "util/validation.h"

#include "log/error.h"
#include "log/logger.h"

#include "uv.h"

#include <string.h>

static ts_error_t
plugin_load_cb(const uv_dirent_t *ent, void *ctx)
{
    int               rc;
    struct ts_plugin  plug;
    ts_plugin_load_cb cb;

    if (ent->type != UV_DIRENT_FILE) {
        return TS_ERR_SUCCESS;
    }

    cb = (ts_plugin_load_cb)ctx;
    log_info("Loading plugin `%s'", ent->name);

    if ((rc = ts_plugin_load(&plug, ent->name)) != 0) {
        log_warn("Could not load plugin `%s'", ent->name);
    } else if ((rc = (*cb)(&plug)) != 0) {
        return rc;
    }

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_plugin_load(struct ts_plugin *plug, const char *filename)
{
    int   rc;
    void *ptr;

    CHECK_NULL_PARAMS_2(plug, filename);

    memset(plug, 0, sizeof(*plug));

    if ((rc = uv_dlopen(filename, &plug->handle)) < 0) {
        log_error("uv_dlopen: %s", uv_dlerror(&plug->handle));
        uv_dlclose(&plug->handle);
        return TS_ERR_LOAD_PLUGIN_FAILED;
    }

    if ((rc = ts_fill_plugins_symoles(plug) != 0)) {
        log_error("ts_fill_plugins_symoles: %s", ts_strerror(rc));
        ts_plugin_unload(plug);
        return TS_ERR_INVALID_PLUGIN;
    }

    log_debug("Initializing plugin: %s, author: %s, version: %s",
              plug->name,
              plug->author,
              plug->version);

    if (plug->init_func && (rc = (*plug->init_func)(NULL)) != 0) {
        log_error(
            "Coud not initialize plugin `%s': %s", plug->name, ts_strerror(rc));
        ts_plugin_unload(plug);
        return TS_ERR_LOAD_PLUGIN_FAILED;
    }

    return TS_ERR_SUCCESS;
}

void
ts_plugin_unload(struct ts_plugin *plug)
{
    if (plug->free_func) {
        (*plug->free_func)();
    }

    uv_dlclose(&plug->handle);
}

ts_error_t
ts_plugin_load_all(const char *dirname, ts_plugin_load_cb cb)
{
    return ts_iterate_directory(dirname, true, &plugin_load_cb, (void *)cb);
}
