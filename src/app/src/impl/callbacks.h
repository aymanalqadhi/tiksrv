#ifndef TIKSRV_APP_IMPL_CALLBACKS_H
#define TIKSRV_APP_IMPL_CALLBACKS_H

#include "net/message.h"
#include "net/tcp_client.h"

#include "interop/plugin.h"

/*!
 * \brief A callback function to be called when a new client connects to the
 *        the server
 *
 * This function is called whenever a new client connects to the server. This
 * function is usually invoked by the application TCP listener
 *
 * \param [in] client  A pointer to the newly connected client
 */
void
ts_on_client_connection(struct ts_tcp_client *client);

/*!
 * \brief A callback function to be called when a connected client disconnects
 *        from the server
 *
 * This function is called whenever a connected client disconnects from  the
 * server. This function is usually invoked by the application TCP listener
 *
 * \param [in] client  A pointer to the disconnected client
 */
void
ts_on_client_disconnection(struct ts_tcp_client *client);

/*!
 * \brief A callback function to be called when a connected client sends
 *        a request to the server
 *
 * \param [in] client  A pointer to the client which sent the request
 * \param [in] req     A pointer to the sent request object
 */
void
ts_on_client_request(struct ts_tcp_client *           client,
                     const struct ts_request_message *req);

#endif
