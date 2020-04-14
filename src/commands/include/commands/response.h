#ifndef TIKSRV_COMMANDS_RESPONSE_H
#define TIKSRV_COMMANDS_RESPONSE_H

#include "log/error.h"

#include <stddef.h>
#include <stdint.h>

enum ts_command_response_code
{
    TS_COMMAND_CODE_SUCCESS = 0,
    TS_COMMAND_CODE_NO_SUCH_COMMAND,
};

struct ts_command_response
{
    uint16_t flags;
    void *   body_buffer;
    size_t   buffer_length, buffer_capacity;
};

/*!
 * \brief Appends \see len bytes from the buffer pointed to by \see buf into a
 *        response object pointed to by \see resp buffer
 *
 * \param [in, out] resp   A pointer to the response object which to write into
 * \param [in]      value  The value which to be written
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_response_write(struct ts_command_response *resp,
                  const void *                buf,
                  uint32_t                    len);

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
ts_response_write_byte(struct ts_command_response *resp, uint8_t value);

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
ts_response_write_uint16(struct ts_command_response *resp, uint16_t value);

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
ts_response_write_uint32(struct ts_command_response *resp, uint32_t value);

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
ts_response_write_uint64(struct ts_command_response *resp, uint64_t value);

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
ts_response_write_string(struct ts_command_response *resp,
                         const char *                str,
                         uint32_t                    len);

#endif
