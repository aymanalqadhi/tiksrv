#ifndef EZTIK_EZTIK_PLUGIN_HPP
#define EZTIK_EZTIK_PLUGIN_HPP

#include "interop/plugin.hpp"
#include "log/logger.hpp"
#include "services/services_manager.hpp"

#include <boost/dll/alias.hpp>

#include <memory>

namespace eztik {

enum class eztik_command : std::uint16_t {
    api_open  = 0x0000u,
    api_close = 0x0001u,
    api_send  = 0x0002u
};

class BOOST_SYMBOL_VISIBLE eztik_plugin final : public ts::interop::plugin {
  public:
    static constexpr std::uint16_t commands_type = 0x01U;

    eztik_plugin()
        : name_ {"ezTik"},
          author_ {"Ayman Al-Qadhi"},
          version_ {"0.1 ALPHA"},
          logger_ {"eztik_plugin"} {
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

    void export_commands(export_func export_cb) const noexcept override;

    void initialize(ts::services::services_manager &services_manager)
        const noexcept override;

    static std::unique_ptr<plugin> create() {
        return std::make_unique<eztik::eztik_plugin>();
    }

  private:
    std::string name_;
    std::string author_;
    std::string version_;

    ts::log::logger logger_;
};

BOOST_DLL_ALIAS(eztik::eztik_plugin::create, create_plugin)

} // namespace eztik

#endif
