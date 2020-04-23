#ifndef TIKSRV_SERVICES_CONTAINER_H
#define TIKSRV_SERVICES_CONTAINER_H

#include "services/hooks_manager.h"

#include "log/error.h"

struct ts_services_container
{
    struct ts_hooks_manager *hooks_manager;
};

struct ts_services_container *
ts_services_container_new(void);

ts_error_t
ts_services_container_init(struct ts_services_container *container);

void
ts_services_container_free(struct ts_services_container *container);

#endif
