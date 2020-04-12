#ifndef TIKSRV_NET_READ_STATE_MACHINE_H
#define TIKSRV_NET_READ_STATE_MACHINE_H

#include "net/message.h"

#include <uv.h>

#include <stddef.h>

struct ts_tcp_client;

typedef void (*ts_read_state_machine_state)(struct ts_tcp_client *, ssize_t);

struct ts_read_state_machine
{
    /* The current state machine state */
    ts_read_state_machine_state state;
    /* The state machine reading callback */
    void (*read_cb)(uv_stream_t *, ssize_t, const uv_buf_t *);
    /* The request header */
    struct ts_request_header header;
    /* The number of read bytes so far */
    size_t nread;
    /* The state machine buffer */
    void *buf;
};

/*!
 * \brief The request message bheader reading state
 *
 * \param [in] client  A pointer to the client currently running this state
 * \param [in] nread   The number of newly read bytes
 */
void
ts_read_header_state(struct ts_tcp_client *client, ssize_t nread);

/*!
 * \brief The request message body reading state
 *
 * \param [in] client  A pointer to the client currently running this state
 * \param [in] nread   The number of newly read bytes
 */
void
ts_read_body_state(struct ts_tcp_client *client, ssize_t nread);

#endif
