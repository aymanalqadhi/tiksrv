#ifndef TIKSRV_SERVICES_HOOKS_MANAGER_H
#define TIKSRV_SERVICES_HOOKS_MANAGER_H

#include <glib.h>

typedef void (*ts_hooks_manager_hook_cb)(const void *, void *data);

typedef enum
{
    TS_HOOKS_MANAGER_HOOK_CONNECTION,
    TS_HOOKS_MANAGER_HOOK_DISCONNECTION,
} ts_hooks_manager_hooks_group_t;

struct ts_hooks_manager
{
    GSList *connection_hooks;
    GSList *disconnection_hooks;
};

/*!
 * \brief Creates a new hooks manager service object
 *
 * \return The created hooks manager service object
 */
struct ts_hooks_manager *
ts_hooks_manager_new(void);

/*!
 * \brief Registers a hook of callback function pointed \see cn, and a context
 *        pointed to by \see data for a specified hooks group indicated by
 *        \see group into a hooks manager service pointed to by \see hm
 *
 * \param [in, out] hm     A pointer to the hooks manager service which to
 *                         register the hook in
 * \param [in]      group  The group in which to register the hook
 * \param [in]      cb     A callback function pointer to be called when the
 *                         hook gets invoked
 * \param [in]      data   A context pointer to be passed to the callback
 *                         function when it gets invoked
 */
void
ts_hooks_manager_register_hook(struct ts_hooks_manager *      hm,
                               ts_hooks_manager_hooks_group_t group,
                               ts_hooks_manager_hook_cb       cb,
                               void *                         data);

/*!
 * \brief Runs a hooks group indicated by \see group in a hooks manager service
 *        pointed to by \see hm
 *
 * \param [in] hm     A pointer to the hooks manager service which hooks to be
 *                    run
 * \param [in] group  The hooks group which to be run
 * \param [in] data   A pointer to data to be passed to hooks callbacks
 *                    functions as the first argument
 */
void
ts_hooks_manager_run_hooks(struct ts_hooks_manager *      hm,
                           ts_hooks_manager_hooks_group_t group,
                           const void *                   data);

/*!
 * \brief Frees the resources used by hooks manager service pointed to by
 *        \see hm
 *
 * \param [in, out] hm  A pointer to the hooks manager service which to be freed
 */
void
ts_hooks_manager_free(struct ts_hooks_manager *hm);

#endif
