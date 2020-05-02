#include "eztik/config.hpp"
#include "eztik/eztik_plugin.hpp"

#include "eztik/commands/api_open.hpp"
#include "services/config_manager.hpp"

#include <functional>
#include <memory>

namespace commands = eztik::commands;

namespace eztik {

void eztik_plugin::export_commands(export_func export_cb) noexcept {

#define EXPORT_COMMAND(type, id, ...)                                          \
    do {                                                                       \
        auto cmd = std::make_unique<type>(__VA_ARGS__);                        \
        export_cb(commands_type, static_cast<std::uint16_t>(id),               \
                  std::move(cmd));                                             \
    } while (0)

    EXPORT_COMMAND(commands::api_open_command, eztik_command::api_open, logger_,
                   sessions_service_);

#undef EXPORT_COMMAND
}

void eztik_plugin::setup_config(
    std::shared_ptr<ts::services::config_manager> config_manager) {
    config_manager->add_option<std::string>(config_keys::ros_ip,
                                            config_defaults::ros_ip);
    config_manager->add_option<std::uint16_t>(config_keys::ros_api_port,
                                              config_defaults::ros_api_port);
    config_manager->add_option<std::string>(config_keys::ros_api_user,
                                            config_defaults::ros_api_user);
    config_manager->add_option<std::string>(config_keys::ros_api_password,
                                            config_defaults::ros_api_password);
}

} // namespace eztik
