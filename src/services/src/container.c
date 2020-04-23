#include "services/container.h"
#include "services/hooks_manager.h"

#include "log/error.h"
#include "log/logger.h"
#include "util/validation.h"

#include <glib.h>

#include <assert.h>

struct ts_services_container *
ts_services_container_new(void)
{
    return g_new0(struct ts_services_container, 1);
}

ts_error_t
ts_services_container_init(struct ts_services_container *container)
{
    CHECK_NULL_PARAMS_1(container);

    container->hooks_manager = ts_hooks_manager_new();

    return TS_ERR_SUCCESS;
}

void
ts_services_container_free(struct ts_services_container *container)
{
    assert(container != NULL);

    ts_hooks_manager_free(container->hooks_manager);
    g_free(container);
}
