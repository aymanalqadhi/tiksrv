#ifndef TIKSRV_NET_TCP_CLIENT_CALLBACKS_H
#define TIKSRV_NET_TCP_CLIENT_CALLBACKS_H

#include <stddef.h>
#include <uv.h>

/*!
 * \brief A callback function to be called when the client has data ready
 *
 * \param [in] socket  A pointer to the client socket
 * \param [in] nread   The read bytes count
 * \param [in] buf     The buffer storing the read bytes
 */
void
ts_tcp_client_read_cb(uv_stream_t *socket, ssize_t nread, const uv_buf_t *buf);

#endif
