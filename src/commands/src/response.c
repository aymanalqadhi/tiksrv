#include "commands/response.h"

#include "log/error.h"
#include "util/validation.h"

#include <endian.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define RESPONSE_DEFAULT_CAPACITY 0x10

static inline ts_error_t
reserve_bytes(struct ts_command_response *resp, size_t n)
{
    void * tmp;
    size_t new_cap;

    if (resp->buffer_length + n <= resp->buffer_capacity) {
        return TS_ERR_SUCCESS;
    }

    new_cap = !resp->buffer_capacity ? RESPONSE_DEFAULT_CAPACITY
                                     : resp->buffer_capacity * 2;

    if (new_cap > UINT32_MAX) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    } else {
        tmp = resp->body_buffer ? realloc(resp->body_buffer, new_cap)
                                : malloc(new_cap);
        if (!tmp) {
            return TS_ERR_MEMORY_ALLOC_FAILED;
        }
    }

    resp->buffer_capacity = new_cap;
    resp->body_buffer     = tmp;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_response_write(struct ts_command_response *resp,
                  const void *                buf,
                  uint32_t                    len)
{
    int rc;

    CHECK_NULL_PARAMS_2(resp, buf);
    CHECK_UNINITIALIZED_DATA_1(resp->body_buffer);

    if ((rc = reserve_bytes(resp, len)) != 0) {
        return rc;
    }

    memcpy(resp->body_buffer + resp->buffer_length, buf, len);
    resp->buffer_length += len;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_response_write_byte(struct ts_command_response *resp, uint8_t value)
{
    int rc;

    CHECK_NULL_PARAMS_1(resp);

    if ((rc = reserve_bytes(resp, 1)) != 0) {
        return rc;
    }

    *((uint8_t *)resp->body_buffer + resp->buffer_length++) = value;
    return TS_ERR_SUCCESS;
}

ts_error_t
ts_response_write_uint16(struct ts_command_response *resp, uint16_t value)
{
    value = htobe16(value);
    return ts_response_write(resp, (void *)&value, sizeof(value));
}

ts_error_t
ts_response_write_uint32(struct ts_command_response *resp, uint32_t value)
{
    value = htobe32(value);
    return ts_response_write(resp, (void *)&value, sizeof(value));
}

ts_error_t
ts_response_write_uint64(struct ts_command_response *resp, uint64_t value)
{
    value = htobe64(value);
    return ts_response_write(resp, (void *)&value, sizeof(value));
}

ts_error_t
ts_response_write_string(struct ts_command_response *resp,
                         const char *                str,
                         uint32_t                    len)
{
    int rc;

    if ((rc = ts_response_write_uint32(resp, len)) != 0) {
        return rc;
    }

    return ts_response_write(resp, (const void *)str, len);
}
