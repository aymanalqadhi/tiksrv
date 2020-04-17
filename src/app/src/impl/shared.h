#ifndef TIKSRV_APP_IMPL_SHARED_H
#define TIKSRV_APP_IMPL_SHARED_H

#include "commands/command.h"

#include <stdint.h>

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
