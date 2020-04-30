#include "core/core_plugin.hpp"

#include "core/commands/echo.hpp"

#include <functional>
#include <memory>

namespace commands = ts::plugins::core::commands;
using export_func_t =
    std::function<void(std::uint32_t, std::unique_ptr<ts::interop::command>)>;

namespace {

template <typename T, typename... TArg>
void export_command(const export_func_t &             export_cb,
                    ts::plugins::core::plugin_command cmd,
                    TArg... args) {
    auto cmd_ptr = std::make_unique<T>(args...);
    export_cb(static_cast<std::uint16_t>(cmd), std::move(cmd_ptr));
}

} // namespace

namespace ts::plugins::core {

void core_plugin::export_commands(export_func_t export_cb) const noexcept {
    ::export_command<commands::echo_command>(export_cb, plugin_command::echo);
}

} // namespace ts::plugins::core
