#include "services/hooks_manager.h"

#include "log/logger.h"

#include <glib.h>

#include <assert.h>

struct hook
{
    ts_hooks_manager_hook_cb cb;
    void *                   data;
};

static void
hook_apply(void *item, const void *data)
{
    struct hook *h = (struct hook *)item;
    (*h->cb)(data, h->data);
}

struct ts_hooks_manager *
ts_hooks_manager_new(void)
{
    return g_new0(struct ts_hooks_manager, 1);
}

void
ts_hooks_manager_register_hook(struct ts_hooks_manager *      hm,
                               ts_hooks_manager_hooks_group_t group,
                               ts_hooks_manager_hook_cb       cb,
                               void *                         data)
{
    struct hook *h = g_new(struct hook, 1);

    h->cb   = cb;
    h->data = data;

    log_debug("Registering a hook in hooks group #%d", group);

    switch (group) {
    case TS_HOOKS_MANAGER_HOOK_CONNECTION:
        hm->connection_hooks = g_slist_append(hm->connection_hooks, h);
        break;
    case TS_HOOKS_MANAGER_HOOK_DISCONNECTION:
        hm->disconnection_hooks = g_slist_append(hm->disconnection_hooks, h);
        break;
    default:
        g_free(h);
        assert(0 && "Unknown hooks group");
    }
}

void
ts_hooks_manager_run_hooks(struct ts_hooks_manager *      hm,
                           ts_hooks_manager_hooks_group_t group,
                           const void *                   data)
{
    log_debug("Running hooks group #%d hooks", group);

    switch (group) {
    case TS_HOOKS_MANAGER_HOOK_CONNECTION:
        g_slist_foreach(
            hm->connection_hooks, (GFunc)&hook_apply, (gpointer)data);
        break;
    case TS_HOOKS_MANAGER_HOOK_DISCONNECTION:
        g_slist_foreach(
            hm->disconnection_hooks, (GFunc)&hook_apply, (gpointer)data);
        break;
    default:
        assert(0 && "Unknown hooks group");
    }
}

void
ts_hooks_manager_free(struct ts_hooks_manager *hm)
{
    assert(hm != NULL);

    g_slist_free_full(hm->connection_hooks, &g_free);
    g_slist_free_full(hm->disconnection_hooks, &g_free);

    g_free(hm);
}
