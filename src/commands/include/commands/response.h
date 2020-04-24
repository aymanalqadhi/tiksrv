#ifndef TIKSRV_COMMANDS_RESPONSE_H
#define TIKSRV_COMMANDS_RESPONSE_H

#include "commands/request.h"
#include "log/error.h"

#include <stddef.h>
#include <stdint.h>

enum
{
    TS_RESPONSE_CODE_SUCCESS         = 0x0000,
    TS_RESPONSE_CODE_NO_SUCH_COMMAND = 0x0001,
    TS_RESPONSE_CODE_UNKNOWN_ERROR   = 0x0002,
} ts_response_code_t;

struct ts_response;

/*!
 * \brief Appends \see len bytes from the buffer pointed to by \see buf into
 *        a response object pointed to by \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write
 *                         into
 * \param [in]      value  The value which to be written
 */
void
ts_response_write(struct ts_response *resp, const void *buf, uint32_t len);
/*!
 * \brief Appends a single byte to a repsonse object pointed to by \see resp
 *        buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 */
void
ts_response_write_byte(struct ts_response *resp, uint8_t value);

/*!
 * \brief Appends a 16 bit unsigned integer to a repsonse object pointed to by
 *        \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 */
void
ts_response_write_uint16(struct ts_response *resp, uint16_t value);

/*!
 * \brief Appends a 32 bit unsigned integer to a repsonse object pointed to by
 *        \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 */
void
ts_response_write_uint32(struct ts_response *resp, uint32_t value);

/*!
 * \brief Appends a 64 bit unsigned integer to a repsonse object pointed to by
 *        \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 */
void
ts_response_write_uint64(struct ts_response *resp, uint64_t value);

/*!
 * \brief Appends a string pointed to by \see str of length \see len to a
 *        response object pointed to by \see resp response buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      str    A pointer to the string which to be appended
 * \param [in]      len    The length of the supplied string
 */
void
ts_response_write_nstring(struct ts_response *resp,
                          const char *        str,
                          uint32_t            len);

/*!
 * \brief Appends a string pointed to by \see str to a response object pointed
 *        to by \see resp response buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      str    A pointer to the string which to be appended
 */
void
ts_response_write_string(struct ts_response *resp, const char *str);

/*!
 * \breif Commits respone pointed to by \see resp contents into a request
 *        pointed to by \see req
 *
 * \param [in, out] resp  A pointer to the response which to be commited
 * \param [in]      req   A pointer to the request into which to commit
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_response_commit(struct ts_response *resp);

/*!
 * \brief Creates a new reponse object
 *
 * \param [in] req  A pointer to the request object which the created response
 *                  object is a reponse to
 *
 * \return  A pointer to the created response object
 */
struct ts_response *
ts_respone_new(const struct ts_request *req);

/*!
 * \brief Gets the flags vlaue associated with a response wrapper object pointed
 *        to by \see req
 *
 * \param [in] req  A pointer to the response object from which to get the value
 *
 * \return The flags value of the supplied response object
 */
uint32_t
ts_response_get_flags(const struct ts_response *resp);

/*!
 * \brief Gets the client id vlaue associated with a response wrapper object
 *        pointed to by \see req
 *
 * \param [in] req  A pointer to the response object from which to get the value
 *
 * \return The client id value of the supplied response object
 */
uint32_t
ts_response_get_client_id(const struct ts_response *resp);

/*!
 * \brief Gets the buffer pointer associated with a response wrapper object
 *        pointed to by \see req
 *
 * \param [in] req  A pointer to the response object from which to get the
 * buffer
 *
 * \return The buffer pointer of the supplied response object
 */
const void *
ts_response_get_buffer(const struct ts_response *resp);

/*!
 * \brief Gets the buffer length of the buffer associated with a response
 * wrapper object pointed to by \see req
 *
 * \param [in] req  A pointer to the response object from which to get the
 * buffer length
 *
 * \return The buffer length of the supplied response object buffer
 */
uint32_t
ts_response_get_length(const struct ts_response *resp);

/*!
 * \brief Gets the buffer capacity of the buffer associated with a response
 *        wrapper object pointed to by \see req
 *
 * \param [in] req  A pointer to the response object from which to get the
 * buffer capacity
 *
 * \return The buffer capacity of the supplied response object buffer
 */
uint32_t
ts_response_get_capacity(const struct ts_response *resp);

/*!
 * \brief Sets the response type value of a response wrapper object pointed
 *        to by \see resp to the value specified with \see flags
 *
 * \param [in, out] resp   A pointer to the response wrapper object which to set
 *                         its flags vlaue
 * \param [in]      type   The new response type value
 */
void
ts_response_set_type(struct ts_response *resp, uint16_t type);

/*!
 * \brief Sets the response code value of a response wrapper object pointed
 *        to by \see resp to the value specified with \see flags
 *
 * \param [in, out] resp   A pointer to the response wrapper object which to set
 *                         its flags vlaue
 * \param [in]      code   The new response code value
 */
void
ts_response_set_code(struct ts_response *resp, uint16_t code);

/*!
 * \brief Sets the flags value of a response wrapper object pointed to by \see
 *        resp to the value specified with \see flags
 *
 * \param [in, out] resp   A pointer to the response wrapper object which to set
 *                         its flags vlaue
 * \param [in]      flags  The new flags value
 */
void
ts_response_set_flags(struct ts_response *resp, uint32_t flags);

/*!
 * \brief Frees the used resources by the response object
 *
 * \param [in, out] resp  A pointer to the response object to be freed
 */
void
ts_response_free(struct ts_response *resp);

#endif
