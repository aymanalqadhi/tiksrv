#include "app/app.h"

#include "config/config.h"
#include "log/logger.h"
#include "log/error.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void
destroy_app_on_sigint(int sig)
{
    log_debug("SIGINT recieved, exiting");
    ts_app_destroy();
    exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
    int              rc;
    struct ts_config config;

    ts_config_parse_argv(&config, argc, argv);
    log_set_level(config.log_level);

    // Ignore SIGPIPE signal produced by connection being closed by remote peer
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, &destroy_app_on_sigint);

    if ((rc = ts_app_run(&config)) != 0) {
        log_fatal("Could not start app: %s", ts_strerror(rc));
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
