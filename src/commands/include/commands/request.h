#ifndef TIKSRV_COMMANDS_REQUEST_H
#define TIKSRV_COMMANDS_REQUEST_H

#include "log/error.h"

#include <stddef.h>
#include <stdint.h>

struct ts_command_request;

/*!
 * \brief Reads \see *len bytes from a request object pointed to by \see req,
 *        and store the read bytes in a buffer pointed to by \see buf
 *
 * \param [in]  req  A pointer to the request object from which to read
 * \param [out] buf  A pointer to the buffer into which to write
 * \param [in]  len  The size of the passed output buffer
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_request_read(struct ts_command_request *req, void *buf, size_t len);

/*!
 * \brief Reads a single bytes from a request object pointed to by \see req,
 *        and store the read value into the pointer \see valptr
 *
 * \param [in]  req     A pointer to the request object from which to read
 * \param [out] valptr  A pointer to the output variable into which to write the
 *                      read value
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_request_read_byte(struct ts_command_request *req, uint8_t *valptr);

/*!
 * \brief Reads a 16 bit unsigned integer from a request object pointed to by
 *       \see req, and store the read value into the pointer \see valptr
 *
 * \param [in]  req     A pointer to the request object from which to read
 * \param [out] valptr  A pointer to the output variable into which to write the
 *                      read value
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_request_read_uint16(struct ts_command_request *req, uint16_t *valptr);

/*!
 * \brief Reads a 32 bit unsigned integer from a request object pointed to by
 *       \see req, and store the read value into the pointer \see valptr
 *
 * \param [in]  req     A pointer to the request object from which to read
 * \param [out] valptr  A pointer to the output variable into which to write the
 *                      read value
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_request_read_uint32(struct ts_command_request *req, uint32_t *valptr);

/*!
 * \brief Reads a 64 bit unsigned integer from a request object pointed to by
 *       \see req, and store the read value into the pointer \see valptr
 *
 * \param [in]  req     A pointer to the request object from which to read
 * \param [out] valptr  A pointer to the output variable into which to write the
 *                      read value
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_request_read_uint64(struct ts_command_request *req, uint64_t *valptr);

/*!
 * \brief Reads a string of of size \see *len from a request object pointed to
 * by \see req, and store the read string in a buffer pointed to by \see buf,
 *        and the read string length in \see len
 *
 * \param [in]      req  A pointer to the request object from which to read
 * \param [out]     buf  A pointer to the output string buffe
 * \param [in, out] len  A pointer to the passed string buffer length
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_request_read_string(struct ts_command_request *req,
                       char *                     buf,
                       uint32_t *                 len);

/*!
 * \brief Initializes a command request wrapper object pointed to by \see req
 *
 * \param [out] req        A pointer to the command request object to be
 *                         initialized
 * \param [in]  client_id  The request issuing client id
 * \param [in]  body_buf   A pointer to the request object body buffer
 * \param [in]  body_len   The length of the passed body buffer
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_request_init(struct ts_command_request *req,
                uint32_t                   client_id,
                const void *               body_buf,
                uint32_t                   body_len);

/*!
 * \brief Gets the client id vlaue associated with a request wrapper object
 *        pointed to by \see req
 *
 * \param [in] req  A pointer to the request object of which to get the value
 *
 * \return The client id value of the supplied request object
 */
uint32_t
ts_command_request_get_client_id(const struct ts_command_request *req);

/*!
 * \brief Gets the flags vlaue associated with a request wrapper object pointed
 *        to by \see req
 *
 * \param [in] req  A pointer to the request object of which to get the value
 *
 * \return The flags value of the supplied request object
 */
uint16_t
ts_command_request_get_flags(const struct ts_command_request *req);

/*!
 * \brief Gets the buffer pointer associated with a request wrapper object
 *        pointed to by \see req
 *
 * \param [in] req  A pointer to the request object of which to get the buffer
 *
 * \return The buffer pointer of the supplied request object
 */
const void *
ts_command_request_get_buffer(const struct ts_command_request *req);

/*!
 * \brief Gets the buffer length of the buffer associated with a request wrapper
 *        object pointed to by \see req
 *
 * \param [in] req  A pointer to the request object of which to get the buffer
 *                  length
 *
 * \return The buffer length of the supplied request object buffer
 */
uint32_t
ts_command_request_get_length(const struct ts_command_request *req);

#endif
