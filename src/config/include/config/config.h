#ifndef TIKSRV_CONFIG_CONFIG_H
#define TIKSRV_CONFIG_CONFIG_H

#include "constants.h"

#include <stdint.h>

struct ts_config
{
    const char *exec_name;  /* The name of the executable */

    uint16_t listen_port;   /* The port on whcih to listen (default: 3434) */
    uint32_t backlog;       /* The maximum number of clients in the accept 
                               queue */

    int  log_level: 3;      /* The logging verbosity level (0-7) */
    int      ipv6 : 1;      /* Use IPv6? (0-1, default: 1) */
};

/*!
 * \brief Parsed command line aguments vector pointed to by \see argv
 *
 * This function parses the application command line arguments vector pointed
 * to by \see argv of length \see argc, and store the parsed configuration
 * values to the configuration object pointed to by \see cfg.
 * This function terminates the application should it detect invalid argument
 * values
 *
 * \param [in] cfg   A pointer to the configuration object into whic to store
 *                   the parsed configuration values
 * \param [in] argc  The arguments vector count
 * \param [in] argv  A pointer to the arguments vector
 */
void
ts_config_parse_argv(struct ts_config *cfg, int argc, char **argv);

#endif
