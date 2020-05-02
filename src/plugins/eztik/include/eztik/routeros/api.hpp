#ifndef EZTIK_ROUTEROS_API_HPP
#define EZTIK_ROUTEROS_API_HPP

#include "eztik/routeros/sentence.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <vector>

namespace eztik::routeros {

class api final {
    using connect_handler =
        std::function<void(const boost::system::error_code &)>;
    using send_handler = std::function<void(const boost::system::error_code &,
                                            const std::size_t &)>;

  public:
    api(boost::asio::io_context &io) : sock_ {io} {
    }

    api(api &&rh) : sock_ {std::move(rh.sock_)} {
    }

    api(const api &rh) = delete;

    ~api() {
        if (is_open()) {
            close();
        }
    }

    void open(const std::string &    host,
              std::uint16_t          port,
              const connect_handler &cb);

    void send(const sentence &s, const send_handler &cb);

    void close();

    inline auto is_open() const noexcept -> bool {
        return sock_.is_open();
    }

  private:
    std::vector<std::uint8_t>    buffer_;
    boost::asio::ip::tcp::socket sock_;
};

} // namespace eztik::routeros

#endif
