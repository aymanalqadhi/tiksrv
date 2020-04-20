#ifndef TIKSRV_INTEROP_PLUGIN_H
#define TIKSRV_INTEROP_PLUGIN_H

#include "commands/command.h"
#include "services/container.h"

#include "config/config.h"
#include "log/error.h"

#include <uv.h>

#include <stddef.h>

#define TS_PLUGIN_NAME_SYM                     "ts_plugin_name"
#define TS_PLUGIN_AUTHOR_SYM                   "ts_plugin_author"
#define TS_PLUGIN_VERSION_SYM                  "ts_plugin_version"
#define TS_PLUGIN_COMMANDS_EXPORT_FUNCTION_SYM "ts_plugin_commands_export"
#define TS_PLUGIN_INIT_FUNCTION_SYM            "ts_plugin_init"
#define TS_PLUGIN_FREE_FUNCTION_SYM            "ts_plugin_free"

struct ts_plugin;

typedef ts_error_t (*ts_plugin_commands_export_func_t)(
    ts_command_export_func_t);
typedef ts_error_t (*ts_plugin_init_func_t)(const struct ts_config *,
                                            struct ts_services_container *);
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
};

/*!
 * \brief Loads a plugin of name \see filename into the plugin object pointed to
 *        by \see plug
 *
 * \param [out] plug  A pointer to the plugin object into whcih to save the
 *                    loaded plugin info
 * \param [in] filename  A pointer to the plugin filename on the filesystem
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_plugin_load(struct ts_plugin *            plug,
               const char *                  filename,
               const struct ts_config *      conf,
               struct ts_services_container *svcs);

/*!
 * \brief Unloads a plugin object pointed to by \see plug freeing all resources
 *        used by it
 *
 * \param [in, out] plug  A pointer to the plugin object which to be unloaded
 */
void
ts_plugin_unload(struct ts_plugin *plug);

/*!
 * \breif Loads all plugins a directory of name \see dirname
 *
 * \param [in] dirname  A pointer to the directory name from which to load the
 *                      plugins
 * \param [in] cb       A callback function pointer to be called on each loaded
 *                      plugin
 *
 * \param [in, out] plug  A pointer to the plugin object which to be unloaded
 */
ts_error_t
ts_plugin_load_all(const char *                  dirname,
                   ts_plugin_load_cb             cb,
                   const struct ts_config *      conf,
                   struct ts_services_container *svcs);

#endif
