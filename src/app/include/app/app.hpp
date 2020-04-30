#ifndef TIKSRV_APP_APP_HPP
#define TIKSRV_APP_APP_HPP

#include "app/session.hpp"

#include "config/config.hpp"
#include "log/logger.hpp"
#include "net/message.hpp"
#include "net/tcp_client.hpp"
#include "net/tcp_server.hpp"

#include <boost/system/error_code.hpp>

#include <cstdint>
#include <memory>
#include <unordered_map>

namespace ts::app {

class tiksrv_app final : public ts::net::tcp_server_handler,
                         public ts::net::tcp_client_handler {
  public:
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

    tiksrv_app(const ts::config::config &conf)
        : conf_ {conf},
          logger_ {"app"},
          server_ {
              conf[ts::config::config_key::listen_port].as<std::uint16_t>(),
              conf[ts::config::config_key::liten_backlog].as<std::uint32_t>(),
              *this, *this} {
    }

    void run();

    void on_accept(std::shared_ptr<ts::net::tcp_client> client) final override;
    void on_error(client_ptr                       client,
                  const boost::system::error_code &err) final override;
    void on_close(client_ptr client) final override;
    void on_request(client_ptr client, ts::net::request &&req) final override;

  private:
    void initialize();

  private:
    const ts::config::config &      conf_;
    ts::log::logger     logger_;
    ts::net::tcp_server server_;

    std::unordered_map<std::uint32_t, session> sessions_;
};

} // namespace ts::app

#endif
