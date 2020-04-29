#ifndef TIKSRV_APP_APP_HPP
#define TIKSRV_APP_APP_HPP

#include "app/config.hpp"

#include "log/logger.hpp"
#include "net/tcp_server.hpp"

#include <cstdint>

namespace ts::app {

class tiksrv_app final {
  public:
    tiksrv_app(const config &conf)
        : conf_ {conf},
          logger_ {"app"},
          server_ {conf[config_key::listen_port].as<std::uint16_t>(),
                   conf[config_key::liten_backlog].as<std::uint32_t>()} {
    }

    void run();

  private:
    void initialize();

  private:
    const config &      conf_;
    ts::log::logger     logger_;
    ts::net::tcp_server server_;
};

} // namespace ts::app

#endif
