#ifndef TIKSRV_APP_SERVER_HPP
#define TIKSRV_APP_SERVER_HPP

#include "config/config.hpp"
#include "log/logger.hpp"

#include "net/tcp_client.hpp"
#include "net/tcp_listener.hpp"

#include "interop/command.hpp"
#include "services/hooks_manager.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>

#include <cstdint>
#include <memory>
#include <utility>

namespace ts::app {

class server final : public ts::net::tcp_listener_handler,
                     public ts::net::tcp_client_handler {
    using error_code = boost::system::error_code;
    using client_ptr = std::shared_ptr<ts::net::tcp_client>;

  public:
    server(const ts::config::config &                   conf,
           boost::asio::io_context &                    io,
           std::shared_ptr<ts::services::hooks_manager> hooks_manger)
        : io_ {io},
          listener_ {
              io_,
              conf[ts::config::config_key::listen_port].as<std::uint16_t>(),
              conf[ts::config::config_key::liten_backlog].as<std::uint32_t>(),
              *this, *this},
          hooks_manager_ {std::move(hooks_manger)},
          logger_ {"server"} {
    }

    inline void run() {
        listener_.start();
    }

    inline void add_command(std::uint32_t                           id,
                            std::unique_ptr<ts::interop::command> &&cmd) {
        commands_.emplace(std::make_pair(id, std::move(cmd)));
    }

    void on_accept(std::shared_ptr<ts::net::tcp_client> client) override;
    void on_error(client_ptr client, const error_code &err) override;
    void on_request(client_ptr client, ts::net::request &&req) override;

  private:
    boost::asio::io_context &                    io_;
    ts::net::tcp_listener                        listener_;
    std::shared_ptr<ts::services::hooks_manager> hooks_manager_;
    ts::log::logger                              logger_;

    std::map<std::uint32_t, std::unique_ptr<ts::interop::command>> commands_;
};

} // namespace ts::app

#endif
