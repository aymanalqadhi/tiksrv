#include "net/tcp_server.hpp"

#include "spdlog/fmt/ostr.h"

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>

#include <cassert>
#include <string>

namespace ip = boost::asio::ip;

using boost::asio::placeholders::error;
using boost::system::error_code;

namespace ts::net {

void tcp_server::start() {
    assert(!running_.load());

    logger_.debug("Starting server on {} with {} backlog",
                  acceptor_.local_endpoint(), backlog_);

    acceptor_.listen(backlog_);
    running_.store(true);
    accept_next();

    io_.run();
}

void tcp_server::stop() {
    assert(running_.load());

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

    auto client_ptr = std::make_shared<tcp_client>(
        current_client_id_.fetch_add(1), io_, clients_handler_);
    acceptor_.async_accept(
        client_ptr->socket(), [this, client_ptr](const auto &err) {
            if (err) {
                logger_.error("Server error: {}", err.message());
            } else {
                server_handler_.on_accept(std::move(client_ptr));
            }

            accept_next();
        });
}

} // namespace ts::net
