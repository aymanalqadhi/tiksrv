#ifndef TIKSRV_NET_TCP_CLIENT_H
#define TIKSRV_NET_TCP_CLIENT_H

#include "log/error.h"
#include "net/message.h"
#include "net/read_state_machine.h"

#include "uthash.h"
#include "uv.h"

#include <stdint.h>

struct ts_tcp_listener;

struct ts_tcp_client
{
    uint32_t                id;           /* The client unique identifier */
    uv_tcp_t                socket;       /* The client connection socket */
    struct ts_tcp_listener *listener;     /* A pointer to the listener which
                                             accepted this client */
    struct ts_read_state_machine read_sm; /* Client data reading state
                                             machine */
    struct ts_response_context response_ctx;

    UT_hash_handle hh;
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

#endif
