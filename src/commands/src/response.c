#include "commands/impl/response.h"
#include "commands/response.h"

#include "log/error.h"
#include "util/validation.h"

#include <endian.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define RESPONSE_MAX_CAPACITY 8192

static inline size_t
calculate_new_capacity(size_t current_cap, size_t current_len, size_t n)
{
    return pow(2, ceil(log2(current_len + n)));
}

static inline ts_error_t
reserve_bytes(struct ts_command_response *resp, size_t n)
{
    void * tmp;
    size_t new_cap;

    if (resp->buffer_length + n <= resp->buffer_capacity) {
        return TS_ERR_SUCCESS;
    }

    if ((new_cap = calculate_new_capacity(resp->buffer_capacity,
                                          resp->buffer_length,
                                          n)) > RESPONSE_MAX_CAPACITY) {
        return TS_ERR_INDEX_OUT_OF_RANGE;
    }

    tmp = resp->body_buffer ? realloc(resp->body_buffer, new_cap)
                            : malloc(new_cap);

    if (!tmp) {
        return TS_ERR_MEMORY_ALLOC_FAILED;
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

    if (len == 0) {
        return TS_ERR_SUCCESS;
    }

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

ts_error_t
ts_respone_init(struct ts_command_response *resp)
{
    CHECK_NULL_PARAMS_1(resp);

    memset((void *)resp, 0, sizeof(*resp));
    return TS_ERR_SUCCESS;
}

uint16_t
ts_command_response_get_flags(const struct ts_command_response *resp)
{
    return resp->flags;
}

const void *
ts_command_response_get_buffer(const struct ts_command_response *resp)
{
    return resp->body_buffer;
}

uint32_t
ts_command_response_get_length(const struct ts_command_response *resp)
{
    return resp->buffer_length;
}

uint32_t
ts_command_response_get_capacity(const struct ts_command_response *resp)
{
    return resp->buffer_capacity;
}

void
ts_command_response_set_flags(struct ts_command_response *resp, uint16_t flags)
{
    resp->flags = flags;
}
