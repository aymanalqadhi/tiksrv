#include "config/config.h"
#include "config/constants.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static inline long
parse_long(const char *str, const char *field, long min, long max)
{
    char *endp;
    long  n;

    endp = NULL;
    n    = strtol(str, &endp, 10);

    if (!endp || *endp) {
        fprintf(stderr, "[!] Invalid %s value: %s\n", field, str);
        exit(EXIT_FAILURE);
    } else if (n < min || n > max) {
        fprintf(stderr,
                "[!] Value of %s has exceeded boundaries (%ld-%ld)\n",
                field,
                min,
                max);
        exit(EXIT_FAILURE);
    }

    return n;
}

void
ts_config_parse_argv(struct ts_config *cfg, int argc, char **argv)
{
    char arg, *endp;
    long parsed_value;

    static struct option options[] = {
        { "port", required_argument, NULL, 'p' },
        { "backlog", required_argument, NULL, 'b' },
        { "ipv6", no_argument, NULL, '6' },
        { "verbose", no_argument, NULL, 'v' },
        { "help", no_argument, NULL, 'h' },
        { "version", no_argument, NULL, 'V' },
    };

    cfg->exec_name   = argv[0];
    cfg->listen_port = TS_CONFIG_DEFAULT_PORT;
    cfg->backlog     = TS_CONFIG_DEFAULT_BACKLOG;
    cfg->ipv6        = TS_CONFIG_DEFAULT_IPV6_ENABLED;
    cfg->log_level   = TS_CONFIG_DEFAULT_LOG_LEVEL;

    while ((arg = getopt_long_only(
                argc, argv, TS_CONFIG_ARGS_LIST, options, NULL)) != -1) {
        switch (arg) {
        case 'h':
            printf("Usage: %s %s\n\n%s\n\nOptions:\n%s\n",
                   argv[0],
                   TS_CONFIG_COMMAND_LINE_USAGE,
                   TS_CONFIG_APP_DESCRIPTION,
                   TS_CONFIG_COMMAND_LINE_HELP);
            exit(EXIT_SUCCESS);
        case 'V':
            printf(
                "%s version %s\n", TS_CONFIG_APP_NAME, TS_CONFIG_APP_VERSION);
            exit(EXIT_SUCCESS);
        case 'p':
            cfg->listen_port = parse_long(optarg, "port", 0, 0xFFFF);
            break;
        case 'b':
            cfg->backlog = parse_long(optarg, "backlog", 0, INT32_MAX);
            break;
        case 'v':
            ++cfg->log_level;
            break;
        case '6':
            cfg->ipv6 = 1;
            break;
        default:
            fprintf(stderr, "[!] Invalid argument value: %s\n", optarg);
            exit(EXIT_FAILURE);
        }
    }
}
