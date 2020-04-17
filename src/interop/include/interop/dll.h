#ifndef TIKSRV_INTRO_DYNAMIC_LINKING_H
#define TIKSRV_INTRO_DYNAMIC_LINKING_H

#include "interop/plugin.h"
#include "log/error.h"

/*!
 * \brief Fills a plugin object pointed to \plug with its exported symboles
 *
 * This function tries to load the plugin pointed to by \plugin publicly
 * exported symboles.
 * Plugins have required and optional symboles. If a required symbole is not
 * found, an error code is returned. While when an optional symbole is not
 * found, the function still succeeds, but a warning is generating in the logs
 *
 * - Required symboles:
 *   1. Name (ts_plugin_name): The name of the plugin.
 * - Optional Symboles:
 *   1. Author (ts_plugin_author): The name of the plugin author
 *   2. Version (ts_plugin_version):  The version of the plugin
 *   3. Init function (ts_plugin_init): A function that initializes the plugin
 *   4. Free function (ts_plugin_free): A function that frees the used
 *        resources by the plugin
 *   5. Commands export function (ts_plugin_commands_export): A function that
 *        exported the plugin commands
 *
 * \param 
 * \return 0 on success, or a negative value indicating error on failure
 */
ts_error_t
ts_fill_plugins_symoles(struct ts_plugin *plug);

#endif
