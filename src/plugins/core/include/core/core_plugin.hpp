#ifndef TIKSRV_PLUGINS_CORE_CORE_PLUGIN_HPP
#define TIKSRV_PLUGINS_CORE_CORE_PLUGIN_HPP

#include "interop/plugin.hpp"

#include <boost/dll/alias.hpp>

#include <memory>

namespace ts::plugins::core {

enum class plugin_command : std::uint16_t { echo = 0x0000UL };

class BOOST_SYMBOL_VISIBLE core_plugin final : public ts::interop::plugin {
  public:
    core_plugin()
        : name_ {"Core Plugin"},
          author_ {"Ayman Al-Qadhi"},
          version_ {"0.1 ALPHA"} {
    }

    auto name() const noexcept -> const std::string & override {
        return name_;
    }

    auto author() const noexcept -> const std::string & override {
        return author_;
    }

    auto version() const noexcept -> const std::string & override {
        return version_;
    }

    void export_commands(
        std::function<void(std::uint32_t,
                           std::unique_ptr<ts::interop::command>)> export_cb)
        const noexcept override;

    static std::unique_ptr<plugin> create() {
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
