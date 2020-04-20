#ifndef TIKSRV_NET_TCP_LISTNER_H
#define TIKSRV_NET_TCP_LISTNER_H

#include "config/config.h"
#include "net/message.h"
#include "net/tcp_client.h"

#include <glib.h>

#include <stddef.h>
#include <uv.h>

#include <stdbool.h>

struct ts_tcp_listener_app_callbacks
{
    void (*on_connection_cb)(struct ts_tcp_client *);
    void (*on_disconnection_cb)(struct ts_tcp_client *);
    void (*on_request_cb)(struct ts_tcp_client *,
                          const struct ts_request_message *);
};

struct ts_tcp_listener
{
    uv_tcp_t socket;
    uint16_t port;
    uint16_t backlog;
    bool     is_running;

    void (*client_disconnect_cb)(uv_stream_t *);
    struct ts_tcp_listener_app_callbacks app_cbs;
    struct ts_tcp_client *               clients;
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
