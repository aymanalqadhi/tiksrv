#include "eztik/session.h"

#include <glib.h>
#include <uv.h>

#include <assert.h>
#include <stdint.h>
#include <string.h>

struct eztik_session *
eztik_session_new(uint32_t id)
{
    struct eztik_session *ret = g_new(struct eztik_session, 1);

    memset((void *)ret, 0, sizeof(*ret));
    ret->id = id;

    return ret;
}

void
eztik_session_close(struct eztik_session *session)
{
    assert(session != NULL);

    if (!uv_is_closing((uv_handle_t *)&session->api.socket)) {
        uv_close((uv_handle_t *)&session->api.socket, NULL);
    }
}

void
eztik_session_free(struct eztik_session *session)
{
    assert(session != NULL);
    g_free(session);
}
