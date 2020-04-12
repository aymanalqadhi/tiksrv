#include "net/memory.h"
#include "net/message.h"

#include <uv.h>

#include <stdlib.h>

void
ts_header_buffer_alloc_cb(uv_handle_t *handle, size_t s_size, uv_buf_t *buf)
{
    buf->len  = TS_MESSAGE_REQUEST_HEADER_SIZE;
    buf->base = malloc(TS_MESSAGE_REQUEST_HEADER_SIZE);
}
