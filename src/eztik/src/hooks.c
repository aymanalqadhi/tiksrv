#include "eztik/hooks.h"

#include "log/logger.h"

#include <stdint.h>

void
eztik_connection_hook(const void *idptr, void *ctx)
{
    uint32_t id = *(const uint32_t *)idptr;

    log_fatal("[EZTIK] New connection #%u", id);
}

void
eztik_disconnection_hook(const void *idptr, void *ctx)
{
    uint32_t id = *(const uint32_t *)idptr;

    log_fatal("[EZTIK] Lost connection #%u", id);
}
