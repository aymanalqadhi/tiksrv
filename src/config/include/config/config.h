#ifndef TIKSRV_CONFIG_CONFIG_H
#define TIKSRV_CONFIG_CONFIG_H

#include "constants.h"

#include <stdint.h>

struct ts_config
{
    const char *exec_name;

    uint16_t listen_port;
    uint32_t backlog;

    int  log_level: 3;
    int      ipv6 : 1;
};

void
ts_config_parse_argv(struct ts_config *cfg, int argc, char **argv);

#endif
