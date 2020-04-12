#include "net/tcp_client.h"

#include "error.h"
#include "log.h"
#include "utils/validation.h"

#include <stdint.h>
#include <stdlib.h>

static inline ts_error_t
tcp_client_init(struct ts_tcp_client *c)
{
    int      rc;
    static uint32_t currnet_id = 0;

    if ((rc = uv_tcp_init(uv_default_loop(), &c->socket)) < 0) {
        log_debug("uv_tcp_init: %s", uv_strerror(rc));
        return TS_ERR_SOCKET_CREATE_FAILED;
    }

    c->id = currnet_id++;
    c->socket.data = c;

    return TS_ERR_SUCCESS;
}

ts_error_t
ts_tcp_client_create(struct ts_tcp_client **c)
{
    int                   rc;
    struct ts_tcp_client *client;

    CHECK_NULL_PARAMS_1(c);

    if (!(client = (struct ts_tcp_client *)malloc(sizeof(*client)))) {
        return TS_ERR_MEMORY_ALLOC_FAILED;
    } else if ((rc = tcp_client_init(client)) != 0) {
        free(client);
        return rc;
    }

    *c = client;
    return TS_ERR_SUCCESS;
}

void
ts_tcp_client_free(struct ts_tcp_client *c)
{
    free(c);
}
