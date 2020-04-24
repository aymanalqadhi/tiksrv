#ifndef TIKSRV_NET_MESSAGE_H
#define TIKSRV_NET_MESSAGE_H

#include "log/error.h"

#include <uv.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TS_MESSAGE_REQUEST_HEADER_SIZE  16
#define TS_MESSAGE_RESPONSE_HEADER_SIZE 16

typedef enum
{
    TS_RESPONSE_TYPE_SYSTEM = 0
} ts_response_type_t;

struct ts_request_header
{
    uint16_t type;
    uint16_t command;
    uint32_t flags;
    uint32_t seq_number;
    uint32_t body_length;
};

struct ts_response_header
{
    uint16_t type;
    uint16_t code;
    uint32_t flags;
    uint32_t seq_number;
    uint32_t body_length;
};

struct ts_request_message
{
    const struct ts_request_header *header;
    const void *                    body;
};

struct ts_response_message
{
    const struct ts_response_header *header;
    const void *                     body;
};

struct ts_response_context
{
    uv_buf_t buffers[2];
    bool     include_body;
};

/*!
 * \brief Encodes a request message header pointed to by \see req into a
 *        binary buffer pointed to by \see buf
 *
 * \param [in]  resp  A pointer to the request header to be encoded
 * \param [out] buf   A pointer to the output buffer
 * \param [in]  len   The length of the passed buffer
 *
 * \return 0 on success, or a negative value indicating error on failure
 */
ts_error_t
ts_encode_request_header(const struct ts_request_header *req,
                         void *                          buf,
                         size_t                          len);

/*!
 * \brief Decodes a buffer into a requet message header pointed to by \see req
 *
 * \param [out] req  A pointer to the output request header object
 * \param [in]  buf  A pointer to the buffer which to be decoded
 * \param [in]  len  The length of the passed buffer
 *
 * \return 0 on success, or a negative value indicating error on failure
 */
ts_error_t
ts_decode_request_header(struct ts_request_header *req,
                         const void *              buf,
                         size_t                    len);

/*!
 * \brief Encodes a response message header pointed to by \see resp into a
 *        binary buffer pointed to by \see buf
 *
 * \param [in]  resp  A pointer to the response header to be encoded
 * \param [out] buf   A pointer to the output buffer
 * \param [in]  len   The length of the passed buffer
 *
 * \return 0 on success, or a negative value indicating error on failure
 */
ts_error_t
ts_encode_response_header(const struct ts_response_header *resp,
                          void *                           buf,
                          size_t                           len);

/*!
 * \brief Decodes a buffer into a response message header pointed to by
 *        \see resp
 *
 * \param [out] req  A pointer to the output response header object
 * \param [in]  buf  A pointer to the buffer which to be decoded
 * \param [in]  len  The length of the passed buffer
 *
 * \return 0 on success, or a negative value indicating error on failure
 */
ts_error_t
ts_decode_response_header(struct ts_response_header *resp,
                          const void *               buf,
                          size_t                     len);

#endif
