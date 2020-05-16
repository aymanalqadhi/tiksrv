#ifndef EZTIK_PLUGIN_HPP
#define EZTIK_PLUGIN_HPP

#include "eztik/config.hpp"
#include "eztik/services/sessions.hpp"

#include "config/config.hpp"
#include "interop/plugin.hpp"
#include "log/logger.hpp"
#include "services/config_manager.hpp"
#include "services/hooks_manager.hpp"
#include "services/services_manager.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/dll/alias.hpp>

#include <memory>

namespace eztik {

enum class eztik_command : std::uint16_t {
    api_open  = 0x0000U,
    api_close = 0x0001U,
    api_send  = 0x0002U
};

class BOOST_SYMBOL_VISIBLE eztik_plugin final : public ts::interop::plugin {
  public:
    static constexpr std::uint16_t commands_type = 0x0001U;

    eztik_plugin(const ts::config::config &                    conf,
                 ts::log::logger &                             logger,
                 boost::asio::io_context &                     io,
                 std::shared_ptr<ts::services::config_manager> config_manager,
                 std::shared_ptr<ts::services::hooks_manager>  hooks_manager)
        : name_ {"ezTik"},
          author_ {"Ayman Al-Qadhi"},
          version_ {"0.1 ALPHA"},
          logger_ {logger},
          sessions_service_ {conf, io, logger, std::move(hooks_manager)} {
        setup_config(std::move(config_manager));
    }

    [[nodiscard]] inline auto name() const noexcept
        -> const std::string & override {
        return name_;
    }

    [[nodiscard]] inline auto author() const noexcept
        -> const std::string & override {
        return author_;
    }

    [[nodiscard]] inline auto version() const noexcept
        -> const std::string & override {
        return version_;
    }

    void export_commands(export_func export_cb) noexcept override;

    void
    setup_config(std::shared_ptr<ts::services::config_manager> config_manager);

    [[nodiscard]] static auto create(ts::services::services_manager &svcs)
        -> std::unique_ptr<plugin> {
        if (!svcs.has<ts::services::hooks_manager>()) {
            throw std::runtime_error {
                "Hooks manager service is required for this plugin"};
        }

        auto hooks_manger   = svcs.get<ts::services::hooks_manager>();
        auto config_manager = svcs.get<ts::services::config_manager>();

        return std::make_unique<eztik::eztik_plugin>(
            svcs.config(), svcs.logger(), svcs.io_context(),
            std::move(config_manager), std::move(hooks_manger));
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
