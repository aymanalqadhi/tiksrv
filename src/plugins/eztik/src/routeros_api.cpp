#include "eztik/routeros/api.hpp"

#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <cassert>

namespace ip = boost::asio::ip;

namespace eztik::routeros {

void api::open(const std::string &    host,
               std::uint16_t          port,
               const connect_handler &cb) {
    assert(!is_open());

    try {
        ip::tcp::endpoint ep {ip::make_address(host), port};
        sock_.async_connect(ep, cb);
    } catch (...) {
        cb(boost::asio::error::bad_descriptor);
    }
}

void api::send(const sentence &s, const send_handler &cb) {
    assert(is_open());

    buffer_.clear();
    s.encode(buffer_);

    sock_.async_send(boost::asio::buffer(buffer_), cb);
}

void api::close() {
    assert(is_open());
    sock_.close();
}

} // namespace eztik::routeros
