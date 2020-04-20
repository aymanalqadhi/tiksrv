#ifndef TIKSRV_NET_TCP_LISTENRER_CALLBACKS_H
#define TIKSRV_NET_TCP_LISTENRER_CALLBACKS_H

#include "net/tcp_client.h"

#include <uv.h>

/*!
 * \brief A callback function to be called when a listener stops listening
 *
 * \param [in, out] handle  A pointer to the listener handle which has stopped
 *                          listening
 */
void
ts_tcp_listener_stop_cb(uv_handle_t *handle);

/*!
 * \brief A callback function to be called when a listener pointed to by
 *        \see stream has accepted a new client
 *
 * \param [in] stream  A pointer to the socket of the listener which has
 *                     accepted the new client
 * \param [in] status  The client acceptance process status
 */
void
ts_tcp_listener_accepted_cb(uv_stream_t *stream, int status);

/*!
 * \brief A callback function to be called when a client which has previously
 *        accepted by the listener pointed to \see stream has closed connection
 *
 * \param [in] stream  A pointer to the socket of the listener which has
 *                     accepted the closed client
 */
void
ts_tcp_listener_disconnected_cb(uv_stream_t *stream);

#endif
