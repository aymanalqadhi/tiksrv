#include "eztik/commands/api_send.hpp"
#include "eztik/commands/response.hpp"

#include "eztik/routeros/api.hpp"
#include "eztik/routeros/sentence.hpp"

#include "impl/proto/api_send.pb.h"

#include "net/message.hpp"

#include <boost/algorithm/string.hpp>

#include <string>
#include <vector>

using boost::system::error_code;

using eztik::commands::messages::ApiRequest;
using eztik::commands::messages::ApiResponse;
using eztik::commands::messages::ApiResponse_ResponseType;
using eztik::routeros::response_sentence;
using eztik::services::session;

namespace eztik::commands {

void api_send_command::execute(client_ptr client, ts::net::request &&req) {
    ApiRequest msg {};

    if (!msg.ParseFromString(req.body())) {
        logger_.error("Client #{} sent an invalid API request", client->id());
        client->respond(ts::net::response_code::invalid_request,
                        req.header().tag);
        return;
    }

    auto sentence = session_->api()->make_request(msg.command());

    for (const auto &word : msg.params()) {
        sentence->push(word);
    }

    session_->api()->send(
        sentence, [this, client {std::move(client)}, tag = req.header().tag](
                      const error_code &err, response_sentence &&resp) {
            if (err) {
                logger_.error("Could not send API message for client #{}: {}",
                              session_->id(), err.message());
                client->respond(ts::net::response_code::unknown_error, tag);
                return false;
            }

            ApiResponse msg {};
            msg.set_type(static_cast<ApiResponse_ResponseType>(resp.type()));

            for (auto &[key, value] : resp) {
                msg.mutable_params()->operator[](std::move(key)) =
                    std::move(value);
            }

            client->respond(msg.SerializeAsString(), tag);

            return !resp.empty();
        });
}

} // namespace eztik::commands
