#ifndef EZTIK_ROUTEROS_API_H
#define EZTIK_ROUTEROS_API_H

#include "log/error.h"

#include <glib.h>
#include <uv.h>

#include <stdbool.h>
#include <stdint.h>

typedef void (*ros_api_open_cb)(bool, void *);
typedef void (*ros_api_close_cb)(bool, void *);
typedef void (*ros_api_write_cb)(bool, void *);

struct ros_api
{
    uv_tcp_t socket;
};

struct ros_sentence
{
    GArray * words;
    uint32_t current_tag;
};

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
 * \brief Closes an already opened RouterOS API connection
 *
 * \param [in, out] api   A pointer to the API connection to be closed
 * \param [in]      cb    A callback function pointer to be invoked when the
 *                        disconnection process is done
 * \param [in]      data  A pointer to be passed to the callback function
 *                        pointer when it gets invoked
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ros_api_close(struct ros_api *api, ros_api_close_cb, void *data);

/*!
 * \brief Writes an API sentence to an opened RouterOS API connection
 *
 * \param [in] api        A pointer to the API connection into which to write
 * \param [in] sentence   A pointer to the API sentence which to be written
 * \param [in] cb         A callback function pointer to be invoked when the
 *                        write process is done
 * \param [in] data       A pointer to be passed to the callback function
 *                        pointer when it gets invoked
 *
 * \return 0 on success, or a negative value indicating error otherwise
 */
ts_error_t
ros_api_write(struct ros_api *           api,
              const struct ros_sentence *sentence,
              ros_api_write_cb           cb,
              void *                     data);

#endif
