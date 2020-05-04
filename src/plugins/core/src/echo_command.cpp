#include "core/commands/echo.hpp"

#include "net/message.hpp"

#include <memory>

namespace ts::plugins::core::commands {

void echo_command::execute(client_ptr client, ts::net::request &&req) {
    client->respond(req.body(), req.header().tag);
}

} // namespace ts::plugins::core::commands
