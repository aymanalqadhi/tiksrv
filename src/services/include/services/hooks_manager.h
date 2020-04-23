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

struct ts_hooks_manager *
ts_hooks_manager_new(void);

void
ts_hooks_manager_register_hook(struct ts_hooks_manager *      hm,
                               ts_hooks_manager_hooks_group_t group,
                               ts_hooks_manager_hook_cb       cb,
                               void *                         data);

void
ts_hooks_manager_run_hooks(struct ts_hooks_manager *      hm,
                           ts_hooks_manager_hooks_group_t group,
                           const void *                   data);

void
ts_hooks_manager_free(struct ts_hooks_manager *hm);

#endif
