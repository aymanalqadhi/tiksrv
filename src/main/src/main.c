#include "app/app.h"

#include "config/config.h"
#include "log/logger.h"
#include "log/error.h"

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int              rc;
    struct ts_config config;

    ts_config_parse_argv(&config, argc, argv);
    log_set_level(config.log_level);

    if ((rc = ts_app_run(&config)) != 0) {
        log_fatal("Could not start app: %s", ts_strerror(rc));
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
