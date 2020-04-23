#include "eztik/eztik.h"

#include "log/error.h"
#include "log/logger.h"
#include "services/hooks_manager.h"
#include "util/validation.h"

#include <glib.h>

#include <assert.h>

struct eztik eztik;

void
eztik_connection_hook(const void *idptr, void *ctx)
{
    /** Does nothing **/
    log_trace("eztik plugin connection hook");
}

void
eztik_disconnection_hook(const void *idptr, void *ctx)
{
    uint32_t id = *(const uint32_t *)idptr;

    if (eztik_has_session(id)) {
        log_info("Destroying client #%u session", id);
        eztik_remove_session(id);
    }
}

ts_error_t
eztik_init(const struct ts_config *cfg, struct ts_services_container *svcs)
{
    CHECK_NULL_PARAMS_1(cfg);

    eztik.config   = cfg;
    eztik.services = svcs;
    eztik.sessions = g_hash_table_new_full(
        g_int_hash, g_int_equal, &g_free, (GDestroyNotify)&eztik_session_free);

    ts_hooks_manager_register_hook(svcs->hooks_manager,
                                   TS_HOOKS_MANAGER_HOOK_CONNECTION,
                                   &eztik_connection_hook,
                                   NULL);
    ts_hooks_manager_register_hook(svcs->hooks_manager,
                                   TS_HOOKS_MANAGER_HOOK_DISCONNECTION,
                                   &eztik_disconnection_hook,
                                   NULL);

    return TS_ERR_SUCCESS;
}

bool
eztik_has_session(uint32_t id)
{
    assert(eztik.sessions != NULL);

    return g_hash_table_contains(eztik.sessions, (gconstpointer)&id);
}

void
eztik_add_session(struct eztik_session *session)
{
    assert(session != NULL);

    g_hash_table_insert(
        eztik.sessions,
        (gpointer)g_memdup((gconstpointer)&session->id, sizeof(session->id)),
        (gpointer)session);
}

void
eztik_remove_session(uint32_t id)
{
    if (!g_hash_table_contains(eztik.sessions, (gconstpointer)&id)) {
        log_warn("An attempt to remove a non existent session");
        return;
    }

    g_hash_table_remove(eztik.sessions, (gconstpointer)&id);
}

void
eztik_destroy(void)
{
    g_hash_table_destroy(eztik.sessions);
}
