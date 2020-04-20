#ifndef TIKSRV_COMMNADS_COMMAND_H
#define TIKSRV_COMMNADS_COMMAND_H

#include "commands/request.h"
#include "commands/response.h"

#include "net/message.h"
#include "net/tcp_client.h"

#include "log/error.h"
#include "services/container.h"

#include "uthash.h"

#include <stdint.h>

enum ts_system_commands
{
    TS_COMMAND_ECHO
};

typedef void (*ts_command_func_t)(struct ts_request *,
                                  struct ts_services_container *);

struct ts_command
{
    uint32_t          command;
    ts_command_func_t func;
    UT_hash_handle    hh;
};

typedef void (*ts_command_export_func_t)(const struct ts_command *);

/*!
 * \brief Exports system commands
 *
 * \param export_func  A callback function pointer to be called on each
 *                     exported command
 *
 * \return 0 on success, or a negative vlaue indicating error on failure
 */

ts_error_t
ts_commands_export(ts_command_export_func_t export_func);
/*!
 * \brief Execute a command pointed to by \see cmd which has been issued by a
 *        client pointed to by \see client, with a request pointed to by \see
 *        req
 *
 * \param [in] cmd    A pointer to the command which to be executed
 * \param [in] client  A pointer to the client which issued the command
 * \param [in] req     A pointer to the command issuing request
 *
 * \return 0 on success, or a negative vlaue indicating error on failure
 */
ts_error_t
ts_command_execute(struct ts_command *              cmd,
                   struct ts_tcp_client *           client,
                   const struct ts_request_message *req);

#endif
