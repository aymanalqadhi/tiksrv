#ifndef TIKSRV_APP_CALLBACKS_H
#define TIKSRV_APP_CALLBACKS_H

#include "net/message.h"
#include "net/tcp_client.h"

void
ts_on_client_connection(struct ts_tcp_client *client);

void
ts_on_client_disconnection(struct ts_tcp_client *client);

void
ts_on_client_request(struct ts_tcp_client *           client,
                     const struct ts_request_message *req);

#endif
