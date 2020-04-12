#ifndef TIKSRV_APP_H
#define TIKSRV_APP_H

#include "config.h"
#include "error.h"

ts_error_t
ts_app_run(const struct ts_config *cfg);

void
ts_app_destroy(void);

#endif
