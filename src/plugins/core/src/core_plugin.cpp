#include "core/core_plugin.hpp"

#include "core/commands/echo.hpp"

#include <functional>
#include <memory>

namespace commands = ts::plugins::core::commands;

namespace ts::plugins::core {

void core_plugin::initialize(ts::services::services_manager &svcs) noexcept {
}

void core_plugin::export_commands(export_func export_cb) const noexcept {

#define EXPORT_COMMAND(type, id, ...)                                          \
    do {                                                                       \
        auto cmd = std::make_unique<type>(__VA_ARGS__);                        \
        export_cb((unsigned short)core_plugin::commands_type,                  \
                  static_cast<std::uint16_t>(id), std::move(cmd));             \
    } while (0)

    EXPORT_COMMAND(commands::echo_command, plugin_command::echo);

#undef EXPORT_COMMAND
}

} // namespace ts::plugins::core
