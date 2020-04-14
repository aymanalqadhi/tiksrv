#ifndef TIKSRV_NET_TCP_CLIENT_CALLBACKS_H
#define TIKSRV_NET_TCP_CLIENT_CALLBACKS_H

#include <stddef.h>
#include <uv.h>

/*!
 * \brief A callback function to be called when a client has data ready
 *
 * \param [in] socket  A pointer to the client socket
 * \param [in] nread   The read bytes count
 * \param [in] buf     The buffer storing the read bytes
 */
void
ts_tcp_client_read_cb(uv_stream_t *socket, ssize_t nread, const uv_buf_t *buf);

/*!
 * \brief A callback function to be called when a client has sent data
 *
 * \param [in] req     A pointer to the send request
 * \param [in] status  The result of the send process
 */
void
ts_tcp_client_write_cb(uv_write_t *req, int status);

/*!
 * \brief A callback function to be called when a client has closed its
 *        connection
 *
 * \param [in] socket  A pointer to the client socket
 */
void
ts_tcp_client_close_cb(uv_handle_t *socket);

#endif
