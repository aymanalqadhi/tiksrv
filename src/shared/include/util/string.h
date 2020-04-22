#ifndef TIKSRV_SHARED_UTIL_STRING_H
#define TIKSRV_SHARED_UTIL_STRING_H

#include <stdint.h>
#include <stdbool.h>

bool
ts_str_to_uint8(const char *str, uint8_t *out);

bool
ts_str_to_uint16(const char *str, uint16_t *out);

bool
ts_str_to_uint32(const char *str, uint32_t *out);

bool
ts_str_to_uint64(const char *str, uint64_t *out);

#endif
