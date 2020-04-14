#include "log/error.h"
#include "util/memory.h"

#include <stdlib.h>
#include <string.h>

void *
ts_memdup(const void *mem, size_t len)
{
    void *buf;

    if (!(buf = malloc(len))) {
        return NULL;
    }

    memcpy(buf, mem, len);
    return buf;
}

int
ts_memxchg(void *a, void *b, size_t len)
{
    void *tmp;

    if (!(tmp = malloc(len))) {
        return TS_ERR_MEMORY_ALLOC_FAILED;
    }

    memcpy(tmp, a, len);
    memcpy(a, b, len);
    memcpy(b, tmp, len);
    free(tmp);

    return TS_ERR_SUCCESS;
}
