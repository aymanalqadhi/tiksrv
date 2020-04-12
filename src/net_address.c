#include "net/address.h"

#include "error.h"
#include "log.h"
#include "utils/validation.h"

#include <uv.h>

#include <netinet/in.h>

#include <errno.h>
#include <string.h>

ts_error_t
ts_addr_to_string(uv_tcp_t *sock, char *buf, size_t len)
{
    int                     rc, addrlen;
    struct sockaddr_storage addr;
    const void *            addrptr;

    CHECK_NULL_PARAMS_2(sock, buf);

    addrlen = sizeof(addr);

    if ((rc = uv_tcp_getsockname(sock, (struct sockaddr *)&addr, &addrlen)) <
        0) {
        log_debug("uv_tcp_getsockname: %s", uv_strerror(rc));
        return TS_ERR_LOAD_ADDRESS_FAILED;
    }

    if (addr.ss_family == AF_INET) {
        addrptr = (void *)&((struct sockaddr_in *)&addr)->sin_addr;
    } else {
        addrptr = (void *)&((struct sockaddr_in6 *)&addr)->sin6_addr;
    }

    if ((rc = uv_inet_ntop(addr.ss_family, addrptr, buf, len)) < 0) {
        log_debug("inet_ntop: %s", uv_strerror(rc));
        return TS_ERR_LOAD_ADDRESS_FAILED;
    }

    return TS_ERR_SUCCESS;
}
