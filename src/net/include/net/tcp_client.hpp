#ifndef TIKSRV_NET_TCP_CLIENT_HPP
#define TIKSRV_NET_TCP_CLIENT_HPP

#include "boost/asio/ip/tcp.hpp"
#include "boost/system/error_code.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace ts::net {

class tcp_client;

class tcp_client_handler {
    virtual void on_error(tcp_client &                     client,
                          const boost::system::error_code &err) = 0;
    virtual void on_close(tcp_client &client)                   = 0;
};

class tcp_client final : public std::enable_shared_from_this<tcp_client> {
  public:
    tcp_client(std::uint32_t            id,
               boost::asio::io_context &io,
               tcp_client_handler &     handler)
        : id_ {id}, sock_ {io}, handler_ {handler} {
    }

    const auto &id() const noexcept {
        return id_;
    }

    auto &socket() noexcept {
        return sock_;
    }

  private:
    std::uint32_t                id_;
    boost::asio::ip::tcp::socket sock_;
    tcp_client_handler &         handler_;
};

} // namespace ts::net

#endif
