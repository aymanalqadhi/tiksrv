#include "net/tcp_client.hpp"

#include <boost/asio/completion_condition.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

#include <cstring>
#include <sstream>
#include <stdexcept>

using boost::asio::transfer_exactly;
using boost::asio::placeholders::bytes_transferred;
using boost::asio::placeholders::error;

namespace ts::net {

void tcp_client::read_next(std::size_t n) {

    if (context().buffer.size() < n) {
        context().buffer.resize(n);
    }

    boost::asio::async_read(
        sock_, boost::asio::buffer(context().buffer, n), transfer_exactly(n),
        boost::bind(&tcp_client::handle_read, shared_from_this(),
                    bytes_transferred, context().buffer, error));
}

void tcp_client::start() {
    if (state() != read_state::idle) {
        throw std::runtime_error {"Client already started"};
    }

    state(read_state::reading_header);
    read_next(request_header::size);
}

void tcp_client::stop() {
    if (state() == read_state::closed) {
        return;
    }

    sock_.close();
    state(read_state::closed);
}

void tcp_client::on_reading_header(std::string_view data) {
    if (data.size() != request_header::size) {
        throw std::runtime_error {"Invalid header size"};
    }

    std::memcpy(reinterpret_cast<void *>(&context().header),
                reinterpret_cast<const void *>(context().buffer.c_str()),
                request_header::size);

    state(read_state::reading_body);
    read_next(context().header.body_size.value());
}

void tcp_client::on_reading_body(std::string_view data) {
    handler_.on_request(shared_from_this(), {std::move(context().header),
                                             std::move(context().buffer)});
    context().reinit();

    state(read_state::reading_header);
    read_next(request_header::size);
}

void tcp_client::on_error(const boost::system::error_code &err) {
    if (err == boost::asio::error::eof) {
        handler_.on_close(shared_from_this());
    } else {
        handler_.on_error(shared_from_this(), err);
    }
}

} // namespace ts::net
