#ifndef TIKSRV_NET_ADDRESS_H
#define TIKSRV_NET_ADDRESS_H

#include "log/error.h"

#include "uv.h"

#include <stddef.h>

/*!
 * \brief Gets the string address representation of the socket pointed to by
 *        \see sock, and store it in the buffer pointed to by \see buf
 *
 * \param [in]  sock  A pointer to the socket which to get its address
 * \param [out] buf   A pointer to the buffer into which to write the socket
 *                    address
 * \param [in]  len   The length of the output buffer
 *
 * \retrun 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_addr_to_string(uv_tcp_t *sock, char *buf, size_t len);

#endif
