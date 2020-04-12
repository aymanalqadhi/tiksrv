#ifndef TIKSRV_NET_MEMORY_H
#define TIKSRV_NET_MEMORY_H

#include <uv.h>

void
ts_read_buffer_alloc_cb(uv_handle_t *handle, size_t s_size, uv_buf_t *buf);

#endif
