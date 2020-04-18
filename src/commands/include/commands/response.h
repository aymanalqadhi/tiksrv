#ifndef TIKSRV_COMMANDS_RESPONSE_H
#define TIKSRV_COMMANDS_RESPONSE_H

#include "commands/request.h"
#include "log/error.h"

#include <stddef.h>
#include <stdint.h>

enum ts_response_code
{
    TS_RESPONSE_CODE_SUCCESS = 0,
    TS_RESPONSE_CODE_NO_SUCH_COMMAND,
    TS_RESPONSE_CODE_UNKNOWN_ERROR,
};

struct ts_response;

/*!
 * \brief Appends \see len bytes from the buffer pointed to by \see buf into
 *        a response object pointed to by \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write
 *                         into
 * \param [in]      value  The value which to be written
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_response_write(struct ts_response *resp, const void *buf, uint32_t len);
/*!
 * \brief Appends a single byte into a repsonse object pointed to by \see resp
 *        buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_response_write_byte(struct ts_response *resp, uint8_t value);

/*!
 * \brief Appends a 16 bit unsigned integer into a repsonse object pointed to by
 *        \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_response_write_uint16(struct ts_response *resp, uint16_t value);

/*!
 * \brief Appends a 16 bit unsigned integer into a repsonse object pointed to by
 *        \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_response_write_uint32(struct ts_response *resp, uint32_t value);

/*!
 * \brief Appends a 32 bit unsigned integer into a repsonse object pointed to by
 *        \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_response_write_uint64(struct ts_response *resp, uint64_t value);

/*!
 * \brief Appends a 64 bit unsigned integer into a repsonse object pointed to by
 *        \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_response_write_string(struct ts_response *resp,
                         const char *        str,
                         uint32_t            len);

/*!
 * \brief Initializes a command response object pointed to by \see resp
 *
 * This function simply zeros the passed object
 *
 * \param [out] resp  A pointer to the command response object to be initialized
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_respone_init(struct ts_response *resp);

/*!
 * \brief Gets the flags vlaue associated with a response wrapper object pointed
 *        to by \see req
 *
 * \param [in] req  A pointer to the response object of which to get the value
 *
 * \return The flags value of the supplied response object
 */
uint32_t
ts_response_get_flags(const struct ts_response *resp);

/*!
 * \brief Gets the buffer pointer associated with a response wrapper object
 *        pointed to by \see req
 *
 * \param [in] req  A pointer to the response object of which to get the buffer
 *
 * \return The buffer pointer of the supplied response object
 */
const void *
ts_response_get_buffer(const struct ts_response *resp);

/*!
 * \brief Gets the buffer length of the buffer associated with a response
 * wrapper object pointed to by \see req
 *
 * \param [in] req  A pointer to the response object of which to get the buffer
 *                  length
 *
 * \return The buffer length of the supplied response object buffer
 */
uint32_t
ts_response_get_length(const struct ts_response *resp);

/*!
 * \brief Gets the buffer capacity of the buffer associated with a response
 *        wrapper object pointed to by \see req
 *
 * \param [in] req  A pointer to the response object of which to get the buffer
 *                  capacity
 *
 * \return The buffer capacity of the supplied response object buffer
 */
uint32_t
ts_response_get_capacity(const struct ts_response *resp);

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

ts_error_t
ts_response_commit(struct ts_response *resp, struct ts_request *req);

#endif
