#include "net/memory.h"
#include "net/read_state_machine.h"
#include "net/tcp_client.h"
#include "net/tcp_listener.h"

#include "log/logger.h"

#include <stdlib.h>

static inline void
set_state(struct ts_read_state_machine *read_sm,
          ts_read_state_machine_state   new_state)
{
    free(read_sm->buf);
    read_sm->buf   = NULL;
    read_sm->nread = 0;
    read_sm->state = new_state;
}

void
ts_read_header_state(struct ts_tcp_client *client, ssize_t nread)
{
    int rc;

    client->read_sm.nread += nread;

    if (client->read_sm.nread == TS_MESSAGE_REQUEST_HEADER_SIZE) {
        if ((rc = ts_decode_request_header(&client->read_sm.header,
                                           client->read_sm.buf,
                                           client->read_sm.nread)) != 0) {
            log_error("ts_decode_request_header: %s", ts_strerror(rc));
            ts_tcp_client_close(client);
            return;
        }
        set_state(&client->read_sm, ts_read_body_state);
    }
}

void
ts_read_body_state(struct ts_tcp_client *client, ssize_t nread)
{
    struct ts_request_message req;

    client->read_sm.nread += nread;

    if (client->read_sm.nread == client->read_sm.header.body_length) {
        if (client->listener->app_cbs.on_request_cb) {
            req.header = &client->read_sm.header;
            req.body   = client->read_sm.buf;
            (*client->listener->app_cbs.on_request_cb)(client, &req);
        }
        set_state(&client->read_sm, ts_read_header_state);
    }
}
