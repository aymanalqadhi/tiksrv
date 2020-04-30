#include "app/app.hpp"

#include "log/core.hpp"
#include "log/logger.hpp"
#include "net/tcp_client.hpp"

#include "spdlog/fmt/ostr.h"

#include <boost/system/error_code.hpp>

#include <atomic>
#include <iostream>
#include <stdexcept>

using boost::system::error_code;

using client_tr = std::shared_ptr<ts::net::tcp_client>;

namespace ts::app {

void tiksrv_app::initialize() {
    logger_.info("Initializing application");
}

void tiksrv_app::run() {
    initialize();

    logger_.info("Running application");

    try {
        server_.start();
    } catch (const std::exception &ex) {
        logger_.error("Could not start server: {}", ex.what());
        throw std::runtime_error {"Could not start application"};
    }
}
void tiksrv_app::on_accept(std::shared_ptr<ts::net::tcp_client> client) {
    logger_.info("Got connection from {} and was assigned #{}",
                 client->endpoint(), client->id());

    client->start();
    sessions_.insert(std::make_pair(client->id(), session {std::move(client)}));
}

void tiksrv_app::on_error(client_tr client, const error_code &err) {
    logger_.warn("An error occured to client #{}: {}", client->id(),
                 err.message());
    on_close(client);
}

void tiksrv_app::on_close(client_tr client) {
    logger_.info("Client #{} has lost connection", client->id());

    if (client->state() != ts::net::read_state::closed) {
        client->stop();
    }

    sessions_.erase(client->id());
}

void tiksrv_app::on_request(client_ptr client, ts::net::request &&req) {
    logger_.trace("New message from #{}:", client->id());
    logger_.trace("[+] Command: 0x{:08X}", req.header().command);
    logger_.trace("[+] Flags: 0x{:08X}", req.header().flags);
    logger_.trace("[+] Tag: 0x{:08X}", req.header().tag);
    logger_.trace("[+] Body Length: 0x{:08X}", req.header().body_size);
}

} // namespace ts::app
