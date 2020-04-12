#ifndef TIKSRV_NET_TCP_CLIENT_H
#define TIKSRV_NET_TCP_CLIENT_H

#include "error.h"
#include "net/message.h"
#include "net/read_context.h"

#include <uthash.h>
#include <uv.h>

#include <stdint.h>

struct ts_tcp_listener;

struct ts_tcp_client
{
    uint32_t                id;
    uv_tcp_t                socket;
    struct ts_tcp_listener *listener;
    struct ts_read_context  read_ctx;

    UT_hash_handle hh;
};

ts_error_t
ts_tcp_client_create(struct ts_tcp_client **c);

void
ts_tcp_client_free(struct ts_tcp_client *c);

#endif
