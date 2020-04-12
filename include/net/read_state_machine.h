#ifndef TIKSRV_NET_READ_STATE_MACHINE_H
#define TIKSRV_NET_READ_STATE_MACHINE_H

#include "net/message.h"

#include <uv.h>

#include <stddef.h>

struct ts_tcp_client;

typedef void (*ts_read_state_machine_state)(struct ts_tcp_client *, ssize_t);

struct ts_read_state_machine
{
    ts_read_state_machine_state state;
    void (*read_cb)(uv_stream_t *, ssize_t, const uv_buf_t *);

    struct ts_request_header header;
    size_t                   nread;
    void *                   buf;
};

void
ts_read_header_state(struct ts_tcp_client *client, ssize_t nread);

void
ts_read_body_state(struct ts_tcp_client *client, ssize_t nread);

#endif
