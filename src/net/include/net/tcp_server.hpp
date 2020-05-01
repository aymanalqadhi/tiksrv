#ifndef TIKSRV_NET_TCP_SERVER_H
#define TIKSRV_NET_TCP_SERVER_H

#include "net/tcp_client.hpp"

#include "log/logger.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <atomic>
#include <cstdint>
#include <memory>

namespace ts::net {

class tcp_server_handler {
  public:
    virtual void on_accept(std::shared_ptr<tcp_client> client) = 0;
};

class tcp_server final {
  public:
    tcp_server(boost::asio::io_context &io,
               std::uint16_t            port,
               std::uint32_t            backlog,
               tcp_server_handler &     server_handler,
               tcp_client_handler &     clients_handler,
               bool                     ipv6 = false)
        : io_ {io},
          acceptor_ {
              io,
              {ipv6 ? boost::asio::ip::tcp::v6() : boost::asio::ip::tcp::v4(),
               port}},
          current_client_id_ {0},
          backlog_ {backlog},
          running_ {false},
          clients_handler_ {clients_handler},
          server_handler_ {server_handler},
          logger_ {"tcp_server"},
          clients_logger_ {"tcp_client"} {
    }

    ~tcp_server() {
        stop();
    }

    void start();
    void stop();

  private:
    void accept_next();

    boost::asio::io_context &      io_;
    boost::asio::ip::tcp::acceptor acceptor_;

    std::atomic_uint32_t current_client_id_;
    std::uint32_t        backlog_;
    std::atomic_bool     running_;

    tcp_server_handler &server_handler_;
    tcp_client_handler &clients_handler_;

    ts::log::logger logger_, clients_logger_;
}; // namespace ts::net

} // namespace ts::net

#endif
