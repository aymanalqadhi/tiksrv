#ifndef TIKSRV_CONFIG_CONSTANTS_H
#define TIKSRV_CONFIG_CONSTANTS_H

#define TS_CONFIG_DEFAULT_CONFIG_FILE         "tiksrv.ini"
#define TS_CONFIG_DEFAULT_PORT                3434
#define TS_CONFIG_DEFAULT_IPV6_ENABLED        0
#define TS_CONFIG_DEFAULT_LOG_LEVEL           2
#define TS_CONFIG_DEFAULT_BACKLOG             1000
#define TS_CONFIG_DEFAULT_LISTEN_IPV4_ADDRESS "0.0.0.0"
#define TS_CONFIG_DEFAULT_LISTEN_IPV6_ADDRESS "::"

#define TS_CONFIG_APP_NAME        "tiksrv"
#define TS_CONFIG_APP_VERSION     "0.1 ALPHA"
#define TS_CONFIG_APP_DESCRIPTION "MikroTik RouterOS managment server"

#define TS_CONFIG_ARGS_LIST "p:b:6l:c:vhV"
#define TS_CONFIG_COMMAND_LINE_USAGE                                           \
    "[ -p port ] [ -b backlog ] [ -6 ] [ -l address ] [ -c config ] [ -v ]\n"  \
    "[ -h ] [ -V ]"

#define TS_CONFIG_COMMAND_LINE_HELP                                            \
    "  -p, --port      The port to listen on.\n"                               \
    "  -b, --backlog   The server listen backlog.\n"                           \
    "  -6, --ipv6      Enforce IPv6 usage.\n"                                  \
    "  -l, --listen    The listening address\n"                                \
    "  -c, --config    Configuration file location\n"                          \
    "  -v, --verbose   Increase the logging level.\n"                          \
    "  -h, --help      Show this message and exit.\n"                          \
    "  -V, --version   Show app version and exit.\n"

#endif
