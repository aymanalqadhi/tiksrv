#include "net/tcp_server.hpp"

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind/bind.hpp>

#include <stdexcept>
#include <string>

namespace ip = boost::asio::ip;

using boost::asio::placeholders::error;
using boost::system::error_code;

namespace ts::net {

void tcp_server::start() {
    if (running_.load()) {
        throw std::runtime_error {"Server is already started"};
    }

    const auto &ep = acceptor_.local_endpoint();
    acceptor_.listen(backlog_);

    logger_.debug("Starting server on {}:{} with {} backlog",
                  ep.address().to_string(), ep.port(), backlog_);

    running_.store(true);
    accept_next();

    io_.run();
}

void tcp_server::stop() {
    if (!running_.load()) {
        throw std::runtime_error {"Server is not running"};
    }

    acceptor_.cancel();
    acceptor_.close();
    io_.stop();

    running_.store(false);
}

void tcp_server::accept_next() {
    if (!running_.load()) {
        logger_.debug("Server is not running");
        return;
    }

    auto client_ptr =
        std::make_shared<tcp_client>(current_client_id_.fetch_add(1), io_);

    acceptor_.async_accept(client_ptr->socket(),
                           boost::bind(&tcp_server::handle_accept, this,
                                       client_ptr,
                                       boost::asio::placeholders::error));
}

void tcp_server::handle_accept(std::shared_ptr<tcp_client> client,
                               const error_code &          err) {
    logger_.fatal("NEW CONNECTION FROM {}",
                  client->socket().remote_endpoint().address().to_string());
    accept_next();
}

} // namespace ts::net
