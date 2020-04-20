#include "config/config.h"
#include "config/constants.h"
#include "config/ini.h"

#include "log/error.h"
#include "log/logger.h"

#include <glib.h>

#include <getopt.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline long
parse_long(const char *str, const char *field, long min, long max)
{
    char *endp;
    long  n;

    endp = NULL;
    n    = strtol(str, &endp, 10);

    if (!endp || *endp) {
        log_error("Invalid %s value: %s\n", field, str);
        exit(EXIT_FAILURE);
    } else if (n < min || n > max) {
        log_error(
            "Value of %s has exceeded boundaries (%ld-%ld)", field, min, max);
        exit(EXIT_FAILURE);
    }

    return n;
}

static int
ini_parse_cb(void *ctx, const char *section, const char *key, const char *value)
{
    char *      tmp;
    GHashTable *config_tbl;
    GHashTable *section_tbl;

    config_tbl = (GHashTable *)ctx;

    if (!(section_tbl = g_hash_table_lookup(config_tbl, section))) {
        section_tbl =
            g_hash_table_new_full(g_str_hash, g_str_equal, &free, &free);
        g_hash_table_insert(
            config_tbl, (gpointer)g_strdup(section), section_tbl);
    }

    g_hash_table_insert(
        section_tbl, (gpointer)g_strdup(key), (gpointer)g_strdup(value));
    log_debug("Configuration entry: [%s] %s = %s", section, key, value);
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
        { "listen", required_argument, NULL, 'l' },
        { "config", required_argument, NULL, 'c' },
        { "verbose", no_argument, NULL, 'v' },
        { "help", no_argument, NULL, 'h' },
        { "version", no_argument, NULL, 'V' },
    };

    cfg->exec_name      = argv[0];
    cfg->listen_port    = TS_CONFIG_DEFAULT_PORT;
    cfg->backlog        = TS_CONFIG_DEFAULT_BACKLOG;
    cfg->listen_address = NULL;
    cfg->config_file    = TS_CONFIG_DEFAULT_CONFIG_FILE;
    cfg->config_table   = NULL;
    cfg->ipv6           = TS_CONFIG_DEFAULT_IPV6_ENABLED;
    cfg->log_level      = TS_CONFIG_DEFAULT_LOG_LEVEL;

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
        case 'l':
            cfg->listen_address = optarg;
            break;
        case 'c':
            cfg->config_file = optarg;
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

    if (!cfg->listen_address) {
        cfg->listen_address = cfg->ipv6 ? TS_CONFIG_DEFAULT_LISTEN_IPV6_ADDRESS
                                        : TS_CONFIG_DEFAULT_LISTEN_IPV4_ADDRESS;
    }
}

ts_error_t
ts_config_parse_config_file(struct ts_config *cfg)
{
    if (!cfg->config_table) {
        cfg->config_table =
            g_hash_table_new_full(g_str_hash,
                                  g_str_equal,
                                  &free,
                                  (GDestroyNotify)g_hash_table_destroy);
    }

    if (ini_parse(cfg->config_file, &ini_parse_cb, (void *)cfg->config_table) <
        0) {
        return TS_ERR_CONFIG_FILE_PARSE_FAILED;
    }

    return TS_ERR_SUCCESS;
}

const char *
ts_config_get_value(const struct ts_config *cfg,
                    const char *            section,
                    const char *            key,
                    const char *            def)
{
    GHashTable *section_table;

    if (!cfg || !cfg->config_table ||
        (section_table = g_hash_table_lookup(cfg->config_table, section))) {
        return def;
    }

    return g_hash_table_lookup(section_table, key);
}

void
ts_config_free(struct ts_config *cfg)
{
    g_hash_table_destroy(cfg->config_table);
}
