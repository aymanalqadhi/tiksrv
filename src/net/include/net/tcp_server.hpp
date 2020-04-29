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

class tcp_server final {
  public:
    tcp_server(std::uint16_t port, std::uint32_t backlog, bool ipv6 = false)
        : current_client_id_ {0},
          backlog_ {backlog},
          running_ {false},
          acceptor_ {
              io_,
              {ipv6 ? boost::asio::ip::tcp::v6() : boost::asio::ip::tcp::v4(),
               port}},
          logger_ {"tcp_server"} {
    }

    ~tcp_server() {
        stop();
    }

    void start();
    void stop();

  private:
    void accept_next();
    void handle_accept(std::shared_ptr<tcp_client>      client,
                       const boost::system::error_code &err);

    std::atomic_uint32_t current_client_id_;
    std::uint32_t        backlog_;
    std::atomic_bool     running_;

    boost::asio::io_context        io_;
    boost::asio::ip::tcp::acceptor acceptor_;

    ts::log::logger logger_;
}; // namespace ts::net

} // namespace ts::net

#endif
