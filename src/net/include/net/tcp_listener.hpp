#ifndef TIKSRV_NET_TCP_LISTENER_H
#define TIKSRV_NET_TCP_LISTENER_H

#include "net/tcp_client.hpp"

#include "log/logger.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <atomic>
#include <cstdint>
#include <memory>

namespace ts::net {

class tcp_listener_handler {
  public:
    virtual void on_accept(std::shared_ptr<tcp_client> client) = 0;
};

class tcp_listener final {
  public:
    tcp_listener(boost::asio::io_context &io,
                 std::uint16_t            port,
                 std::uint32_t            backlog,
                 tcp_listener_handler &   listener_handler,
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
          listener_handler_ {listener_handler},
          clients_handler_ {clients_handler},
          logger_ {"tcp_listener"} {
    }

    inline ~tcp_listener() {
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

    tcp_listener_handler &listener_handler_;
    tcp_client_handler &  clients_handler_;

    ts::log::logger logger_;
}; // namespace ts::net

} // namespace ts::net

#endif
