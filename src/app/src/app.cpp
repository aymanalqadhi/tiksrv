#include "app/app.hpp"

#include "interop/plugin_loader.hpp"
#include "log/core.hpp"
#include "log/logger.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

using ts::config::config_key;

namespace {

inline auto make_command_id(std::uint16_t type, std::uint16_t key) {
    return (type << 16) | (key & 0xFFFF);
}

} // namespace

namespace ts::app {

void tiksrv_app::load_services() {
    logger_.info("Initializing services");

    services_manager_.register_service(config_manager_);
    services_manager_.register_service(hooks_manager_);
    services_manager_.initialize();
}

void tiksrv_app::load_plugins() {
    logger_.info("Loading plugins");

    auto plugins_path = conf_[config_key::plugins_path].as<std::string>();
    logger_.debug("Loading plugins from path: {}", plugins_path);

    ts::interop::plugin_loader loader {};
    auto plugins = loader.load_all(plugins_path, services_manager_);

    for (auto itr = plugins.begin(); itr != plugins.end(); ++itr) {
        logger_.debug("Loading commands from plugin `{}'", (*itr)->name());
        try {
            (*itr)->export_commands([this](auto type, auto key, auto &&cmd) {
                logger_.debug("Loaded command {:#04x}:{:#04x}: {}", type, key,
                              cmd->name());
                server_.add_command(::make_command_id(type, key),
                                    std::move(cmd));
            });
        } catch (const std::exception &ex) {
            logger_.warn("Could not initialize plugin `{}': {}", (*itr)->name(),
                         ex.what());
            plugins.erase(itr);
        }
    }

    plugins_ = std::move(plugins);
}

void tiksrv_app::load_config() {
    logger_.info("Loading configuration");

    try {
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

void tiksrv_app::run() {
    try {
        logger_.info("Initializing application");
        load_services();
        load_plugins();
        load_config();

        logger_.info("Running application");
        server_.run();
    } catch (const std::exception &ex) {
        throw std::runtime_error {
            std::string {"Could not start application: "} + ex.what()};
    }
}

} // namespace ts::app
