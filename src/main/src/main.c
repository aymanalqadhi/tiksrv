#include "app/app.h"

#include "config/config.h"
#include "log/logger.h"
#include "log/error.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

static jmp_buf app_exit_jmp;

static void
destroy_app_on_sigint(int sig)
{
    log_debug("SIGINT recieved, exiting");
    longjmp(app_exit_jmp, 1);
}

int
main(int argc, char *argv[])
{
    int              rc;
    struct ts_config config;

    ts_config_parse_argv(&config, argc, argv);
    log_set_level(config.log_level);

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, &destroy_app_on_sigint);

    if (setjmp(app_exit_jmp)) {
        goto app_exit;
    }

    if ((rc = ts_app_run(&config)) != 0) {
        log_fatal("Could not start app: %s", ts_strerror(rc));
        return EXIT_FAILURE;
    }

app_exit:
    ts_app_destroy();
    return EXIT_SUCCESS;
}
