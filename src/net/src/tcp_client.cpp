#include "net/tcp_client.hpp"

#include <boost/asio/completion_condition.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/system/error_code.hpp>

#include <array>
#include <cassert>
#include <cstring>
#include <memory>
#include <sstream>
#include <string_view>

using boost::asio::transfer_exactly;
using boost::asio::placeholders::bytes_transferred;
using boost::asio::placeholders::error;

#include <spdlog/fmt/bin_to_hex.h>
namespace ts::net {

void tcp_client::read_next(std::size_t n) {
    context().resize(n);

    boost::asio::async_read(
        sock_, boost::asio::buffer(context().buffer(), n), transfer_exactly(n),
        [this](const auto &err, auto nread) {
            handle_read(err, {context().buffer().c_str(), nread});
        });
}

void tcp_client::start() {
    assert(state() == read_state::idle);

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
    assert(data.size() == request_header::size);

    context().header().decode(data.data(), data.size());

    if (context().header().body_size > max_allowed_body_size) {
        logger_.warn(
            "Client #{} exceeded the message size limit (sent: {}, limit: {})",
            id_, context().header().body_size, max_allowed_body_size);
        state(read_state::closed);
        handler_.on_close(shared_from_this());
        return;
    }

    if (context().header().body_size > 0) {
        state(read_state::reading_body);
        read_next(context().header().body_size);
    } else {
        handler_.on_request(
            shared_from_this(),
            {std::move(context().header()), std::move(context().buffer())});
        read_next(request_header::size);
    }
}

void tcp_client::on_reading_body(std::string_view data) {
    handler_.on_request(shared_from_this(), {std::move(context().header()),
                                             std::move(context().buffer())});

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

void tcp_client::send_next(std::shared_ptr<response> resp) {
    std::array<std::uint8_t, response_header::size> header_buf {};
    resp->header().encode(header_buf);

    std::array<boost::asio::const_buffer, 2> send_buffers {
        boost::asio::buffer(header_buf), boost::asio::buffer(resp->body())};

    sock_.async_send(send_buffers, [this](const auto &err, auto sent) {
        if (err) {
            on_error(err);
            return;
        }

        send_enqueued();
    });
}

void tcp_client::enqueue_response(std::shared_ptr<response> resp) {
    send_queue_.push_back(std::move(resp));
}

void tcp_client::send_enqueued() {
    if (send_queue_.empty()) {
        return;
    }

    send_next(send_queue_.front());
    send_queue_.pop_front();
}

void tcp_client::respond(std::shared_ptr<response> resp) {
    if (send_queue_.empty()) {
        send_next(std::move(resp));
    } else {
        enqueue_response(std::move(resp));
        send_enqueued();
    }
}

} // namespace ts::net
