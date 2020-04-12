#ifndef TIKSRV_NET_TCP_LISTNER_H
#define TIKSRV_NET_TCP_LISTNER_H

#include "config/config.h"
#include "net/message.h"
#include "net/tcp_client.h"

#include <stddef.h>
#include <uv.h>

#include <stdbool.h>

struct ts_tcp_listener_app_callbacks
{
    /* The callback function pointer which to be called when the listener has
     * accepted a new client */
    void (*on_connection_cb)(struct ts_tcp_client *);
    /* The callback function pointer which to be called when a a previously
     * accepted client has closed its connection */
    void (*on_disconnection_cb)(struct ts_tcp_client *);
    /* The callback function pointer which to be called when a previously
     * accepted client has sent a request message */
    void (*on_request_cb)(struct ts_tcp_client *,
                          const struct ts_request_message *);
};

struct ts_tcp_listener
{
    uv_tcp_t socket;  /* The listener TCP socket */
    uint16_t port;    /* The listner listening port */
    uint16_t backlog; /* The maximum pending connections in the accpet queue */
    bool is_running;  /* An indicator of the current listener running status */

    /* The callback function pointer which to be called by the client which are
     * closing their connections */
    void (*client_disconnect_cb)(uv_stream_t *);
    /* The application-exposed callback functions pointers */
    struct ts_tcp_listener_app_callbacks app_cbs;
    /* The currently-connect accepted clients */
    struct ts_tcp_client *clients;
};

/*!
 * \brief Creates and initializes a new listener and assign the created object
 *        into the pointer \see outlistener
 *
 * \param [out] outlistener  A pointer to the output listener pointer to be
 *                           assigned the created listener object
 * \param [in]  app_cbs      A pointer to a collection of exposed function
 *                           pointers by the app
 * \param [in]  cfg          A pointer to a configuration object to use to
 *                           configure the created listener
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_listener_create(struct ts_tcp_listener **             outlistener,
                       struct ts_tcp_listener_app_callbacks *app_cbs,
                       const struct ts_config *              cfg);

/*!
 * \brief Starts the listener listening loop
 *
 * \param [in] listener  A pointer to the listener which to be started
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_listener_start(struct ts_tcp_listener *listener);

/*!
 * \brief Stops the listener listening loop
 *
 * \param [in] listener  A pointer to the listener which to be stopped
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ts_tcp_listener_stop(struct ts_tcp_listener *listener);

/*!
 * \brief Frees the resources held by a listener object pointed to \see listener
 *
 * \param [in, out] client  A ponter to the listener to be freed
 */
void
ts_tcp_listener_free(struct ts_tcp_listener *listener);

#endif
