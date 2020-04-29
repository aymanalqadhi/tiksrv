#include "app/app.hpp"

#include "log/core.hpp"
#include "log/logger.hpp"
#include "net/tcp_client.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

#include <boost/system/error_code.hpp>

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

void tiksrv_app::on_error(client_tr client, const error_code &err) {
    logger_.warn("An error occured to client #{}: {}", client->id(),
                 err.message());
}

void tiksrv_app::on_close(client_tr client) {
    logger_.info("Client #{} has lost connection", client->id());
}

void tiksrv_app::on_request(client_ptr client, ts::net::request &&req) {
    logger_.fatal("REQUEST FROM {}:{}",
                  client->endpoint().address().to_string());
}

} // namespace ts::app
