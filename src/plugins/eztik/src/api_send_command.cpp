#include "eztik/commands/api_send.hpp"
#include "eztik/response.hpp"
#include "eztik/routeros/api.hpp"
#include "eztik/routeros/sentence.hpp"

#include "net/message.hpp"

#include "spdlog/fmt/bin_to_hex.h"
#include <boost/algorithm/string.hpp>

#include <string>
#include <vector>

using boost::system::error_code;
using eztik::session;
using eztik::routeros::response_sentence;

namespace eztik::commands {

void api_send_command::execute(client_ptr client, ts::net::request &&req) {
    if (!sessions_svc_.has(client->id())) {
        logger_.warn(
            "Client #{} with no open sessions tried to send an API message",
            client->id());
        client->respond(eztik::make_response_code(
                            eztik::response_code::ros_session_not_open),
                        req.header().tag);
        return;
    }

    ///
    /// TODO:
    /// COMPLETE IMPLEMENTATION
    ///
    auto session = sessions_svc_[client->id()];

    std::vector<std::string> words {};

    auto body = req.body();
    body.erase(0, 4);

    eztik::routeros::request_sentence sent {body, req.header().tag};

    session->send(
        sent,
        [this](const error_code &err, eztik::session &s,
               response_sentence &&resp) {
            logger_.fatal("{}", err.message());
            for (const auto &p : resp) {
                logger_.fatal("{} ===> {}", p.first, p.second);
            }
        },
        true);
}

} // namespace eztik::commands
