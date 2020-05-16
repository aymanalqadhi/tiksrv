#ifndef TIKSRV_PLUGINS_CORE_CORE_PLUGIN_HPP
#define TIKSRV_PLUGINS_CORE_CORE_PLUGIN_HPP

#include "interop/plugin.hpp"
#include "services/services_manager.hpp"

#include <boost/dll/alias.hpp>

#include <memory>

namespace ts::plugins::core {

enum class plugin_command : std::uint16_t { echo = 0x0000U };

class BOOST_SYMBOL_VISIBLE core_plugin final : public ts::interop::plugin {
  public:
    static constexpr std::uint16_t commands_type = 0x00U;

    core_plugin()
        : name_ {"Core Plugin"},
          author_ {"Ayman Al-Qadhi"},
          version_ {"0.1 ALPHA"} {
    }

    [[nodiscard]] auto name() const noexcept -> const std::string & override {
        return name_;
    }

    [[nodiscard]] auto author() const noexcept -> const std::string & override {
        return author_;
    }

    [[nodiscard]] auto version() const noexcept
        -> const std::string & override {
        return version_;
    }

    void export_commands(export_func export_cb) noexcept override;

    [[nodiscard]] static auto create(ts::services::services_manager &svcs)
        -> std::unique_ptr<plugin> {
        return std::make_unique<ts::plugins::core::core_plugin>();
    }

  private:
    std::string name_;
    std::string author_;
    std::string version_;
};

BOOST_DLL_ALIAS(ts::plugins::core::core_plugin::create, create_plugin)

} // namespace ts::plugins::core

#endif
