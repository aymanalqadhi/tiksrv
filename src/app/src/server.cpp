#include "app/server.hpp"

#include "net/message.hpp"
#include "services/hooks_manager.hpp"

#include <boost/system/error_code.hpp>
#include "spdlog/fmt/ostr.h"

#include <memory>

using boost::system::error_code;

namespace ts::app {

void server::on_accept(std::shared_ptr<ts::net::tcp_client> client) {
    try {
        logger_.info("Got connection from {} and was assigned id #{}",
                     client->endpoint(), client->id());
        client->start();
        hooks_manager_->run_hooks(ts::services::hooks_group::connection,
                                  std::move(client));
    } catch (const std::exception &ex) {
        logger_.warn("An error occrued while accepting client #{}: {}",
                     client->id(), ex.what());
    }
}

void server::on_error(client_ptr client, const error_code &err) {
    if (err == boost::asio::error::eof) {
        logger_.info("Client #{} has lost connection", client->id());
    } else {
        logger_.warn("An error occured to client #{}: {}", client->id(),
                     err.message());
    }

    hooks_manager_->run_hooks(ts::services::hooks_group::disconnection,
                              std::move(client));
}

void server::on_request(client_ptr client, ts::net::request &&req) {
    logger_.trace("New message from #{}:", client->id());
    logger_.trace("[+] Command: 0x{:08X}", req.header().command);
    logger_.trace("[+] Flags: 0x{:08X}", req.header().flags);
    logger_.trace("[+] Tag: 0x{:08X}", req.header().tag);
    logger_.trace("[+] Body Length: 0x{:08X}", req.header().body_size);

    if (auto itr = commands_.find(req.header().command);
        itr != commands_.end()) {
        (*itr->second)(std::move(client), std::move(req));
    } else {
        logger_.warn("Client #{} has requested a non-existent command {:#08x}",
                     client->id(), req.header().command);
        client->respond(ts::net::response_code::unknown_command,
                        req.header().tag);
    }
}

} // namespace ts::app
