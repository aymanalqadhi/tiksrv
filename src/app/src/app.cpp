#include "app/app.hpp"

#include "interop/plugin_loader.hpp"
#include "log/core.hpp"
#include "log/logger.hpp"
#include "net/tcp_client.hpp"

#include "spdlog/fmt/ostr.h"

#include <boost/system/error_code.hpp>

#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

using boost::system::error_code;
using ts::config::config_key;

using client_tr = std::shared_ptr<ts::net::tcp_client>;

namespace ts::app {

void tiksrv_app::initialize() {
    logger_.info("Initializing application");

    logger_.info("Initializing services");

    services_manager_.register_service(config_manager_);
    services_manager_.register_service(hooks_manager_);
    services_manager_.initialize();

    logger_.info("Loading plugins");
    load_plugins();

    try {
        logger_.info("Loading configuration");
        auto config_path = conf_[config_key::config_file].as<std::string>();

        if (!std::filesystem::exists(config_path)) {
            std::ofstream {config_path};
        }

        conf_.parse_config_file(config_path);
        conf_.parse_config_file(config_path, config_manager_->options());
    } catch (const std::exception &ex) {
        logger_.warn("Could not parse configuration file: {}", ex.what());
    }
}

void tiksrv_app::load_plugins() {
    auto plugins_path = conf_[config_key::plugins_path].as<std::string>();
    auto loader       = ts::interop::plugin_loader {};

    logger_.debug("Loading plugins from path: {}", plugins_path);
    auto plugins = loader.load_all(plugins_path, services_manager_);

    for (auto itr = plugins.begin(); itr != plugins.end(); ++itr) {
        try {
            logger_.debug("Loading commands from plugin `{}'", (*itr)->name());
            (*itr)->export_commands(
                [this](std::uint16_t type, std::uint16_t key,
                       std::unique_ptr<ts::interop::command> &&command) {
                    logger_.debug("Loaded command {:#04x}:{:#04x}: {}", type,
                                  key, command->name());
                    auto id = (type << 16) | (key & 0xFFFF);
                    commands_.insert(
                        std::make_pair(std::move(id), std::move(command)));
                });
        } catch (const std::exception &ex) {
            logger_.warn("Could not initialize plugin `{}': {}", (*itr)->name(),
                         ex.what());
            plugins.erase(itr);
            continue;
        }
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
    } catch (const std::exception &ex) {
        logger_.warn("An error occrued while accepting client #{}: {}",
                     client->id(), ex.what());
    }

    hooks_manager_->run_hooks(ts::services::hooks_group::connection, client);
}

void tiksrv_app::on_error(client_tr client, const error_code &err) {
    if (err == boost::asio::error::eof) {
        logger_.info("Client #{} has lost connection", client->id());
    } else {
        logger_.warn("An error occured to client #{}: {}", client->id(),
                     err.message());
    }

    hooks_manager_->run_hooks(ts::services::hooks_group::disconnection, client);
}

void tiksrv_app::on_request(client_ptr client, ts::net::request &&req) {
    logger_.trace("New message from #{}:", client->id());
    logger_.trace("[+] Command: 0x{:08X}", req.header().command);
    logger_.trace("[+] Flags: 0x{:08X}", req.header().flags);
    logger_.trace("[+] Tag: 0x{:08X}", req.header().tag);
    logger_.trace("[+] Body Length: 0x{:08X}", req.header().body_size);

    if (auto itr = commands_.find(req.header().command);
        itr != commands_.end()) {
        (*itr->second)(client, std::move(req));
    } else {
        logger_.warn("Client #{} has requested a non-existent command {:#08x}",
                     client->id(), req.header().command);
        client->respond(ts::net::response_code::unknown_command,
                        req.header().tag);
    }
}

} // namespace ts::app
