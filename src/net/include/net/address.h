#ifndef TIKSRV_NET_ADDRESS_H
#define TIKSRV_NET_ADDRESS_H

#include "log/error.h"

#include <uv.h>

ts_error_t
ts_addr_to_string(uv_tcp_t *sock, char *buf, size_t len);

#endif
