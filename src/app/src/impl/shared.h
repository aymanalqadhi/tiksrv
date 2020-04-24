#ifndef TIKSRV_APP_IMPL_SHARED_H
#define TIKSRV_APP_IMPL_SHARED_H

#include "commands/command.h"
#include "config/config.h"
#include "interop/plugin.h"
#include "net/tcp_listener.h"
#include "services/container.h"

#include <glib.h>

#include <stdint.h>

#define MAKE_COMMAND_ID(type, cmd) ((type << 0x10) | (cmd & 0xFFFF))

extern struct ts_app
{
    struct ts_config *            config;
    struct ts_tcp_listener *      listener;
    struct ts_services_container *services;
    GHashTable *                  plugins;
    GHashTable *                  commands;
} app;

/*!
 * \brief Gest an exported command by id
 *
 * \param [in] id  The id of the command which to get
 *
 * \return A pointer to the command if found, or NULL otherwise
 */
struct ts_command *
ts_app_get_command(uint32_t id);

#endif
