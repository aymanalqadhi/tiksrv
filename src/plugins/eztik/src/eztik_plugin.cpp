#include "eztik/eztik_plugin.hpp"

#include "eztik/commands/api_open.hpp"

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

} // namespace eztik
