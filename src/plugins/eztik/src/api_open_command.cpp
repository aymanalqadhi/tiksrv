#include "eztik/commands/api_open.hpp"
#include "eztik/response.hpp"
#include "eztik/routeros/api.hpp"
#include "net/message.hpp"

#include "net/message.hpp"

using rc = eztik::response_code;

namespace eztik::commands {

void api_open_command::execute(client_ptr client, ts::net::request &&req) {
    if (sessions_svc_.has_ready(client->id())) {
        logger_.warn(
            "Client #{} with an already open session has requested a new one",
            client->id());
        client->respond(eztik::make_response_code(rc::ros_session_already_open),
                        req.header().tag);
    } else if (sessions_svc_.has(client->id())) {
        logger_.warn("Client #{} sent multiple session opening requests",
                     client->id());
        client->respond(ts::net::response_code::operation_pending,
                        req.header().tag);
    } else {
        sessions_svc_.create(
            client->id(),
            [this, client, tag = req.header().tag](auto session) {
                if (session) {
                    logger_.info(
                        "A new session was opened successfully for client #{}",
                        client->id());
                    client->respond(ts::net::response_code::success, tag);
                } else {
                    logger_.error("Could not open session for client #{}",
                                  client->id());
                    client->respond(
                        eztik::make_response_code(rc::ros_session_open_failed),
                        tag);
                }
            },
            [client, tag = req.header().tag](auto session) {
                client->respond(
                    eztik::make_response_code(rc::ros_session_closed), tag);
            });
    }
}

} // namespace eztik::commands
