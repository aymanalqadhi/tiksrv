#include "eztik/commands/api_open.hpp"
#include "eztik/response.hpp"
#include "eztik/routeros/api.hpp"

#include "net/message.hpp"

namespace eztik::commands {

void api_open_command::operator()(client_ptr client, ts::net::request &&req) {
    if (sessions_svc_.has(client->id())) {
        logger_.warn(
            "Client #{} with an already open session has requested a new one",
            client->id());
        client->respond(eztik::make_response_code(
                            eztik::response_code::ros_session_already_open),
                        req.header().tag);
        return;
    }

    sessions_svc_.create(client->id(), [this, client,
                                        tag = req.header().tag](auto session) {
        if (session) {
            logger_.info("A new session was opened successfully for client #{}",
                         client->id());
            client->respond(ts::net::response_code::success, tag);
        } else {
            logger_.error("Could not open session for client #{}",
                          client->id());
            client->respond(eztik::make_response_code(
                                eztik::response_code::ros_session_open_failed),
                            tag);
        }
    });
}

} // namespace eztik::commands