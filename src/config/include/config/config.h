#ifndef TIKSRV_CONFIG_CONFIG_H
#define TIKSRV_CONFIG_CONFIG_H

#include "config/constants.h"

#include "log/error.h"
#include "log/logger.h"

#include <glib.h>

#include <stdint.h>

struct ts_config
{
    const char *exec_name;
    const char *config_file;
    const char *listen_address;

    uint16_t listen_port;
    uint32_t backlog;

    int log_level : 3;
    int ipv6 : 1;

    GHashTable *config_table;
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

/*!
 * \brief Parses the configuration file specified in a configuration object
 *        pointed to by \see cfg
 *
 * This function parses the configuration file specified in the configuration
 * object pointed to by \see cfg. The file must be in a valid INI format
 * in order to be parsed successfully.
 *
 * \param [in, out] cfg  A pointer to the configuration object which to parse
 *                       the configuration file for
 *
 * \return 0 on success, or a negative value indicating error on failure
 */
ts_error_t
ts_config_parse_config_file(struct ts_config *cfg);

/*!
 * \brief Gets a configuration value from a configuration object pointed to
 *        by \see cfg
 *
 * \param [in] cfg      A pointer to the configuration object which to look into
 * \param [in] section  The name of the section in which to look for the
 *                      configuration value
 * \param [in] key      The key of the configuration value
 * \param [in] def      A default value to be return in case no matches were
 *                      found
 *
 * \return The configuration value if found, or \see def otheriwse
 */
const char *
ts_config_get_value(const struct ts_config *cfg,
                    const char *            section,
                    const char *            key,
                    const char *            def);

/*!
 * \brief Gets an 8 bit unsigned integer from a configuration object pointed to
 *        by \see cfg
 *
 * \param [in] cfg      A pointer to the configuration object which to look into
 * \param [in] section  The name of the section in which to look for the
 *                      configuration value
 * \param [in] key      The key of the configuration value
 * \param [in] def      A default value to be return in case no matches were
 *                      found
 *
 * \return The configuration value if found, or \see def otheriwse
 */
uint8_t
ts_config_get_uint8(const struct ts_config *cfg,
                    const char *            section,
                    const char *            key,
                    uint8_t                 def);

/*!
 * \brief Gets an 16 bit unsigned integer from a configuration object pointed to
 *        by \see cfg
 *
 * \param [in] cfg      A pointer to the configuration object which to look into
 * \param [in] section  The name of the section in which to look for the
 *                      configuration value
 * \param [in] key      The key of the configuration value
 * \param [in] def      A default value to be return in case no matches were
 *                      found
 *
 * \return The configuration value if found, or \see def otheriwse
 */
uint16_t
ts_config_get_uint16(const struct ts_config *cfg,
                     const char *            section,
                     const char *            key,
                     uint16_t                def);

/*!
 * \brief Gets an 32 bit unsigned integer from a configuration object pointed to
 *        by \see cfg
 *
 * \param [in] cfg      A pointer to the configuration object which to look into
 * \param [in] section  The name of the section in which to look for the
 *                      configuration value
 * \param [in] key      The key of the configuration value
 * \param [in] def      A default value to be return in case no matches were
 *                      found
 *
 * \return The configuration value if found, or \see def otheriwse
 */
uint32_t
ts_config_get_uint32(const struct ts_config *cfg,
                     const char *            section,
                     const char *            key,
                     uint32_t                def);

/*!
 * \brief Gets an 64 bit unsigned integer from a configuration object pointed to
 *        by \see cfg
 *
 * \param [in] cfg      A pointer to the configuration object which to look into
 * \param [in] section  The name of the section in which to look for the
 *                      configuration value
 * \param [in] key      The key of the configuration value
 * \param [in] def      A default value to be return in case no matches were
 *                      found
 *
 * \return The configuration value if found, or \see def otheriwse
 */
uint64_t
ts_config_get_uint64(const struct ts_config *cfg,
                     const char *            section,
                     const char *            key,
                     uint64_t                def);

/*!
 * \brief Frees the resources used by a configuration object
 *
 * \param [in, out] cfg  A pointer to the configuration object which to free
 */
void
ts_config_free(struct ts_config *cfg);

#endif
