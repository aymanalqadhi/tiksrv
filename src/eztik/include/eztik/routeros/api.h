#ifndef EZTIK_ROUTEROS_API_H
#define EZTIK_ROUTEROS_API_H

#include "eztik/routeros/api_sentence.h"

#include "log/error.h"

#include <glib.h>
#include <uv.h>

#include <stdbool.h>
#include <stdint.h>

typedef void (*ros_api_open_cb)(bool, void *);
typedef void (*ros_api_close_cb)(bool, void *);
typedef void (*ros_api_send_cb)(bool, struct ros_api_sentence *, void *);
typedef void (*ros_api_free_cb)(void *);

typedef enum
{
    EZTIK_ROS_API_STATE_DISCONNECTED,
    EZTIK_ROS_API_STATE_CLOSING,
    EZTIK_ROS_API_STATE_CONNECTING,
    EZTIK_ROS_API_STATE_CONNECTED,
    EZTIK_ROS_API_STATE_READING,
    EZTIK_ROS_API_STATE_WRITING,
} ros_api_state_t;

struct ros_api
{
    uv_tcp_t        socket;
    ros_api_state_t state;
};

struct ros_api *
ros_api_new(void);

/*!
 * \brief Opens a RouterOS API connection
 *
 * \param [in, out] api   A pointer to the API connection into whcih to save the
 *                        opened connection
 * \param [in]      cb    A callback function pointer to be invoked when the
 *                        connection process is done
 * \param [in]      data  A pointer to be passed to the callback function
 *                        pointer when it gets invoked
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ros_api_open(struct ros_api *api, ros_api_open_cb cb, void *data);

/*!
 * \brief Sends an API sentence to an opened RouterOS API connection
 *
 * \param [in] api        A pointer to the API connection which to send to
 * \param [in] sentence   A pointer to the API sentence which to be sent
 * \param [in] cb         A callback function pointer to be invoked when the
 *                        send process is done
 * \param [in] data       A pointer to be passed to the callback function
 *                        pointer when it gets invoked
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ros_api_send(struct ros_api *               api,
             struct ros_api_sentence *sentence,
             ros_api_send_cb               cb,
             void *                         data);

/*!
 * \brief Frees the used resources used by a RouterOS API connection pointed to
 *        by \see api closing the connection in the process
 *
 * \param [in, out] api   A pointer to the RouterOS API object to be freed
 * \param [in]      cb    A callback function poiner to be called when the API
 *                        connection is freed
 * \param [in]      data  A pointer to a context data to be passed to the
 *                        supplied callback function
 */
void
ros_api_free(struct ros_api *api, ros_api_free_cb cb, void *data);

#endif
