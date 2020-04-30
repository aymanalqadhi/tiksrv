#include "core/commands/echo.hpp"

#include "net/message.hpp"

#include <memory>

namespace ts::plugins::core::commands

{

void echo_command::operator()(client_ptr client, ts::net::request &&req) {
    auto resp_ptr = std::make_shared<ts::net::response>();

    resp_ptr->body(req.body());
    resp_ptr->tag(req.header().tag);

    client->respond(std::move(resp_ptr));
}

} // namespace ts::plugins::core::commands
