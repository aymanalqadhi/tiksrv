#include "eztik/commands/session_required.hpp"
#include "eztik/response.hpp"
#include "eztik/routeros/api.hpp"

namespace eztik::commands {

void session_required_command::operator()(client_ptr         client,
                                          ts::net::request &&req) {
    if (!sessions_svc_.has_ready(client->id())) {
        logger_.warn(
            "Client #{} with no open sessions tried to send an API message",
            client->id());
        client->respond(eztik::make_response_code(
                            eztik::response_code::ros_session_not_open),
                        req.header().tag);
        return;
    }

    session_ = sessions_svc_[client->id()];
    execute(std::move(client), std::move(req));
}

} // namespace eztik::commands
