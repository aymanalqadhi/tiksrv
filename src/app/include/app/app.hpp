#ifndef TIKSRV_APP_APP_HPP
#define TIKSRV_APP_APP_HPP

#include "app/server.hpp"

#include "config/config.hpp"
#include "log/logger.hpp"

#include "interop/plugin.hpp"
#include "interop/plugin_loader.hpp"

#include "services/config_manager.hpp"
#include "services/hooks_manager.hpp"
#include "services/services_manager.hpp"

#include <boost/asio/io_context.hpp>

#include <cstdint>
#include <memory>
#include <unordered_map>

namespace ts::app {

class tiksrv_app final {
  public:
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

    tiksrv_app(ts::config::config &conf)
        : services_manager_ {conf, io_},
          config_manager_ {std::make_shared<ts::services::config_manager>()},
          hooks_manager_ {std::make_shared<ts::services::hooks_manager>()},
          conf_ {conf},
          io_ {},
          server_ {conf_, io_, hooks_manager_},
          logger_ {"app"} {
    }

    void run();

  private:
    void load_services();
    void load_plugins();
    void load_config();

    ts::services::services_manager                services_manager_;
    std::shared_ptr<ts::services::config_manager> config_manager_;
    std::shared_ptr<ts::services::hooks_manager>  hooks_manager_;

    ts::config::config &    conf_;
    boost::asio::io_context io_;
    server                  server_;

    ts::log::logger                          logger_;
    std::vector<ts::interop::plugin_wrapper> plugins_;
};

} // namespace ts::app

#endif
