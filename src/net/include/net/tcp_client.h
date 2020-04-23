#ifndef TIKSRV_NET_TCP_CLIENT_H
#define TIKSRV_NET_TCP_CLIENT_H

#include "log/error.h"
#include "net/message.h"
#include "net/read_state_machine.h"

#include <glib.h>
#include <uv.h>

#include <stdbool.h>
#include <stdint.h>

struct ts_tcp_listener;

typedef enum
{
    TS_TCP_CLIENT_STATE_DISCONNECTED,
    TS_TCP_CLIENT_STATE_CONNECTED,
    TS_TCP_CLIENT_STATE_DISCONNECTING,
} ts_tcp_client_state_t;

struct ts_write_context
{
    struct ts_tcp_client *      client;
    struct ts_response_message *response;
    uv_buf_t                    buffers[2];
    bool                        has_body;
};

struct ts_tcp_client
{
    uint32_t              id;
    uint32_t              ref_count;
    ts_tcp_client_state_t state;

    uv_tcp_t socket;

    struct ts_tcp_listener *     listener;
    struct ts_read_state_machine read_sm;

    GQueue *write_queue;
};

/*!
 * \brief Creates and initializes a new client and assign the created client
 *        into the pointer \see outclient
 *
 * \param [out] outclient  A pointer to the output client pointer to be assigned
 *                         the created client object
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_client_create(struct ts_tcp_client **outclient);

void
ts_tcp_client_ref(struct ts_tcp_client *client);

void
ts_tcp_client_unref(struct ts_tcp_client *client);

/*!
 * \brief Starts reading loop on the client pointed to by \see client
 *
 * This function starts an asynchronous reading loop on the client pointed to
 * by \see client
 *
 * \param [in] client  A pointer to the client which to start reading on
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_client_start_read(struct ts_tcp_client *client);

/*!
 * \brief Enqueues a response to be sent
 *
 * This function enqueues a request to be sent in order when a call to \see
 * ts_tcp_client_send_equeued is made.
 *
 * \param [in] client  A pointer to the client which to enqueue the response for
 * \param [in] resp    A pointer to the response object to be enqueued
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_client_enqueue_response(struct ts_tcp_client *      client,
                               struct ts_response_message *resp);

/*!
 * \brief Sends the enqueued response messages
 *
 * \param [in] client  A pointer to the client whose requests to be sent
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_client_send_equeued(struct ts_tcp_client *client);

/*!
 * \brief Sends a response message pointed to by \see resp to a client
 *        pointed to by \see client
 *
 * \param [in] client  A pointer to the client which to send the response to
 * \param [in] resp    A pointer to the response message which to be sent
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_client_respond(struct ts_tcp_client *      client,
                      struct ts_response_message *resp);
/*!
 * \brief Creates a response message object with a status code of \see code, and
 *        a sequence number of \see seq_no, then call \see ts_tcp_client_respond
 *        on the created response message
 *
 * \param [in] client  A pointer to the client which to send the response to
 * \param [in] code    The response status code
 * \param [in] seq_no  The conversation sequence number
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_client_respond_with_code(struct ts_tcp_client *client,
                                uint16_t              code,
                                uint32_t              seq_no);

/*!
 * \brief Closes a client pointed to by \see client connection
 *
 * \param [in, out] client  A pointer to the client which connection to be
 *                          closed
 */
void
ts_tcp_client_close(struct ts_tcp_client *client);

/*!
 * \brief Frees the resources held by a client object pointed to \see client
 *
 * \param [in, out] client  A ponter to the client to be freed
 */
void
ts_tcp_client_free(struct ts_tcp_client *client);

/*!
 * \brief Creates a response writing context, then initializes it with a call
 *        to \see ts_write_context_init
 *
 * \param [out] outctx  A pointer to the output response write context object
 * \param [in]  client  A pointer to the client into whose socket to write
 * \param [in]  resp    A pointer to the response object to use
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_write_context_create(struct ts_write_context **  outctx,
                        struct ts_tcp_client *      client,
                        struct ts_response_message *resp);

/*!
 * \brief Initializes a write context object pointed to by \see ctx with
 *        a write message object pointed to by \see resp
 *
 * \param [out] ctx     A pointer to the write context object to be initialized
 * \param [in]  client  A pointer to the client into whose socket to write
 * \param [in]  resp    A pointer to the response object to use
 *
 * \return 0 on success, or a negative value indicating error on failure
 */
ts_error_t
ts_write_context_init(struct ts_write_context *   ctx,
                      struct ts_tcp_client *      client,
                      struct ts_response_message *resp);

/*!
 * \brief Frees the resources used by a write context object
 *
 * \param [in, out] ctx  A pointer to the write context object to be used
 */
void
ts_write_context_free(struct ts_write_context *ctx);

#endif
