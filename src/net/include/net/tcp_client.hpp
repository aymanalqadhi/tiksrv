#ifndef TIKSRV_NET_TCP_CLIENT_HPP
#define TIKSRV_NET_TCP_CLIENT_HPP

#include "net/message.hpp"
#include "net/read_state_machine.hpp"

#include "boost/asio/ip/tcp.hpp"
#include "boost/system/error_code.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace ts::net {

class tcp_client;

struct tcp_client_handler {
    using client_ptr = std::shared_ptr<tcp_client>;

    virtual void on_error(client_ptr                       client,
                          const boost::system::error_code &err) = 0;
    virtual void on_close(client_ptr client)                    = 0;
    virtual void on_request(client_ptr client, request &&req)   = 0;
};

class tcp_client final : public std::enable_shared_from_this<tcp_client>,
                         public read_state_machine {
  public:
    tcp_client(std::uint32_t            id,
               boost::asio::io_context &io,
               tcp_client_handler &     handler)
        : id_ {id}, sock_ {io}, handler_ {handler} {
    }

    void start();
    void stop();

    inline const auto &id() const noexcept {
        return id_;
    }

    inline auto &socket() noexcept {
        return sock_;
    }

    inline auto endpoint() const noexcept {
        return sock_.remote_endpoint();
    }

  protected:
    void on_reading_header(std::string_view data) final override;
    void on_reading_body(std::string_view data) final override;
    void on_error(const boost::system::error_code &err) final override;

  private:
    void read_next(std::size_t n);

  private:
    std::uint32_t                id_;
    boost::asio::ip::tcp::socket sock_;
    tcp_client_handler &         handler_;
};

} // namespace ts::net

#endif
