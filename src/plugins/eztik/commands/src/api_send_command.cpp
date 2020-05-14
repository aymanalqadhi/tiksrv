#include "eztik/commands/api_send.hpp"
#include "eztik/commands/response.hpp"

#include "eztik/routeros/api.hpp"
#include "eztik/routeros/sentence.hpp"

#include "net/message.hpp"

#include "spdlog/fmt/bin_to_hex.h"
#include <boost/algorithm/string.hpp>

#include <string>
#include <vector>

using boost::system::error_code;
using eztik::services::session;
using eztik::routeros::response_sentence;

namespace eztik::commands {

void api_send_command::execute(client_ptr client, ts::net::request &&req) {
    ///
    /// TODO:
    /// COMPLETE IMPLEMENTATION
    ///
    std::vector<std::string> words {};

    auto body = req.body();
    body.erase(0, 4);

    auto sent = session_->api()->make_request(std::move(body));

    session_->api()->send(
        sent,
        [this](const error_code &err, eztik::routeros::api &api,
               response_sentence &&resp) {
            logger_.fatal("{}", err.message());
            for (const auto &p : resp) {
                logger_.fatal("{} ===> {}", p.first, p.second);
            }
        },
        true);
}

} // namespace eztik::commands
