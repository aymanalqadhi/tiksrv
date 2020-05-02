#ifndef EZTIK_EZTIK_PLUGIN_HPP
#define EZTIK_EZTIK_PLUGIN_HPP

#include "eztik/services/sessions.hpp"

#include "config/config.hpp"
#include "interop/plugin.hpp"
#include "log/logger.hpp"
#include "services/hooks_manager.hpp"
#include "services/services_manager.hpp"

#include <boost/asio/io_context.hpp>
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
    static constexpr std::uint16_t commands_type = 0x0001U;

    eztik_plugin(const ts::config::config &                   conf,
                 ts::log::logger &                            logger,
                 boost::asio::io_context &                    io,
                 std::shared_ptr<ts::services::hooks_manager> hooks_manager)
        : name_ {"ezTik"},
          author_ {"Ayman Al-Qadhi"},
          version_ {"0.1 ALPHA"},
          logger_ {logger},
          sessions_service_ {conf, io, logger, std::move(hooks_manager)} {
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

    void export_commands(export_func export_cb) noexcept override;

    static std::unique_ptr<plugin>
    create(ts::services::services_manager &svcs) {
        if (!svcs.has<ts::services::hooks_manager>()) {
            throw std::runtime_error {
                "Hooks manager service is required for this plugin"};
        }

        return std::make_unique<eztik::eztik_plugin>(
            svcs.config(), svcs.logger(), svcs.io_context(),
            svcs.get<ts::services::hooks_manager>());
    }

  private:
    std::string name_;
    std::string author_;
    std::string version_;

    ts::log::logger &                 logger_;
    eztik::services::sessions_service sessions_service_;
};

BOOST_DLL_ALIAS(eztik::eztik_plugin::create, create_plugin)

} // namespace eztik

#endif
