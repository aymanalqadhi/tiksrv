#ifndef TIKSRV_NET_TCP_CLIENT_HPP
#define TIKSRV_NET_TCP_CLIENT_HPP

#include "boost/asio/ip/tcp.hpp"

#include <cstdint>
#include <memory>

namespace ts::net {

class tcp_client final : public std::enable_shared_from_this<tcp_client> {
  public:
    tcp_client(std::uint32_t id, boost::asio::io_context &io)
        : id_ {id}, sock_ {io} {
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
};

} // namespace ts::net

#endif
