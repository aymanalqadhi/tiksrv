#include "app/app.hpp"

#include "interop/plugin_loader.hpp"
#include "log/core.hpp"
#include "log/logger.hpp"
#include "net/tcp_client.hpp"

#include "spdlog/fmt/ostr.h"

#include <boost/system/error_code.hpp>

#include <atomic>
#include <iostream>
#include <stdexcept>

using boost::system::error_code;
using ts::config::config_key;

using client_tr = std::shared_ptr<ts::net::tcp_client>;

namespace ts::app {

void tiksrv_app::initialize() {
    logger_.info("Initializing application");

    logger_.info("Loading plugins");
    load_plugins();
}

void tiksrv_app::load_plugins() {
    auto plugins_path = conf_[config_key::plugins_path].as<std::string>();
    auto loader       = ts::interop::plugin_loader {};

    logger_.debug("Loading plugins from path: {}", plugins_path);
    auto plugins = loader.load_all(plugins_path);

    for (const auto &plugin : plugins) {
        logger_.debug("Loading commands from plugin `{}'", plugin->name());
        plugin->export_commands([this](auto key, auto command) {
            logger_.debug("Loading command #{:x}: {}", key, command->name());
            commands_.insert(
                std::make_pair(std::move(key), std::move(command)));
        });
    }

    plugins_ = std::move(plugins);
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
    try {
        logger_.info("Got connection from {} and was assigned #{}",
                     client->endpoint(), client->id());
        client->start();
        clients_.insert(std::make_pair(client->id(), client));
    } catch (const std::exception &ex) {
        logger_.warn("An error occrued while accepting client #{}: {}",
                     client->id(), ex.what());
    }
}

void tiksrv_app::on_error(client_tr client, const error_code &err) {
    logger_.warn("An error occured to client #{}: {}", client->id(),
                 err.message());
    on_close(client);
}

void tiksrv_app::on_close(client_ptr client) {
    logger_.info("Client #{} has lost connection", client->id());

    if (client->state() != ts::net::read_state::closed) {
        client->stop();
    }

    clients_.erase(client->id());
}

void tiksrv_app::on_request(client_ptr client, ts::net::request &&req) {
    logger_.trace("New message from #{}:", client->id());
    logger_.trace("[+] Command: 0x{:08X}", req.header().command);
    logger_.trace("[+] Flags: 0x{:08X}", req.header().flags);
    logger_.trace("[+] Tag: 0x{:08X}", req.header().tag);
    logger_.trace("[+] Body Length: 0x{:08X}", req.header().body_size);
}

} // namespace ts::app
