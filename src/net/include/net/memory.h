#ifndef TIKSRV_NET_MEMORY_H
#define TIKSRV_NET_MEMORY_H

#include "uv.h"

/*!
 * \brief Allocates a proper buffer depending on the current reading state
 *
 * This function creates a buffer for the next client read operation, whcih
 * size will be determined based on in which state the client is.
 *
 * \param [in] handle  A pointer to the client socket handle
 * \param [in] s_size  libuv suggested size for the buffer
 * \param [out] buf    A ponter to the libuv buffer in which to store the
 *                     allocated buffer
 */
void
ts_read_buffer_alloc_cb(uv_handle_t *handle, size_t s_size, uv_buf_t *buf);

#endif
