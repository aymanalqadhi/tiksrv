#include "util/string.h"

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#define PARSE_INTERGER(str, min, max, out)                                     \
    char *endp = NULL;                                                         \
    long  n    = strtol(str, &endp, 10);                                       \
    if (!endp || *endp || n < min || n > max) {                                \
        return false;                                                          \
    }                                                                          \
    *(out) = n;                                                                \
    return true;

#define TS_STR_TO_UINT_IMPL(size)                                              \
    bool ts_str_to_uint##size(const char *str, uint##size##_t *out)            \
    {                                                                          \
        PARSE_INTERGER(str, 0, UINT##size##_MAX, out);                         \
    }

TS_STR_TO_UINT_IMPL(8)
TS_STR_TO_UINT_IMPL(16)
TS_STR_TO_UINT_IMPL(32)
TS_STR_TO_UINT_IMPL(64)
