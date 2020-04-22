#ifndef EZTIK_EZTIK_H
#define EZTIK_EZTIK_H

#include "eztik/config.h"
#include "eztik/session.h"

#include "config/config.h"
#include "log/error.h"
#include "services/container.h"

#include <glib.h>

#include <stdbool.h>

#define EZTIK_CONFIG(key, def)                                                 \
    (ts_config_get_value(eztik.config, EZTIK_CONFIG_SECTION, key, def))
#define EZTIK_CONFIG_UINT8(key, def)                                           \
    (ts_config_get_uint8(eztik.config, EZTIK_CONFIG_SECTION, key, def))
#define EZTIK_CONFIG_UINT16(key, def)                                          \
    (ts_config_get_uint16(eztik.config, EZTIK_CONFIG_SECTION, key, def))
#define EZTIK_CONFIG_UINT32(key, def)                                          \
    (ts_config_get_uint32(eztik.config, EZTIK_CONFIG_SECTION, key, def))
#define EZTIK_CONFIG_UINT64(key, def)                                          \
    (ts_config_get_uint64(eztik.config, EZTIK_CONFIG_SECTION, key, def))

extern struct eztik
{
    const struct ts_config *      config;
    struct ts_services_container *services;

    GHashTable *sessions;
} eztik;

/*!
 * \brief Initializes the main plugin object
 *
 * \param [in] cfg   A pointer to the application configuration object
 * \param [in] svcs  A pointer to the application services container
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
eztik_init(const struct ts_config *cfg, struct ts_services_container *svcs);

/*!
 * \brief Checks if a session is already registered
 *
 * \param [in] id  The session id to be checked
 *
 * \return true if the session is registered, false if not
 */
bool
eztik_has_session(uint32_t id);

/*!
 * \brief Register a new session
 *
 * \param [in] session  A pointer to the session to be registered
 */
void
eztik_add_session(struct eztik_session *session);

/*!
 * \brief Frees the resources used by this plugin
 */
void
eztik_destroy(void);

#endif
