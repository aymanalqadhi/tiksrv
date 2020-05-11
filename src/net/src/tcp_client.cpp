#include "net/tcp_client.hpp"

#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/system/error_code.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <memory>
#include <sstream>
#include <string_view>

using boost::asio::placeholders::bytes_transferred;
using boost::asio::placeholders::error;

#include <spdlog/fmt/bin_to_hex.h>
namespace ts::net {

void tcp_client::read_next(std::size_t n) {
    context().resize(n);

    boost::asio::async_read(
        sock_, boost::asio::buffer(context().buffer(), n),
        [this, self = shared_from_this()](const auto &err, auto nread) {
            handle_read(err, {context().buffer().c_str(), nread});
        });
}

void tcp_client::start() {
    assert(state() == read_state::idle);

    state(read_state::reading_header);
    read_next(request_header::size);
}

void tcp_client::close() {
    assert(state() != read_state::closed);

    sock_.shutdown(boost::asio::socket_base::shutdown_both);
    sock_.close();

    state(read_state::closed);
}

void tcp_client::on_reading_header(std::string_view data) {
    assert(data.size() == request_header::size);

    context().header().decode(data.data(), data.size());

    if (context().header().body_size > max_allowed_body_size) {
        on_error(boost::asio::error::message_size);
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
    handler_.on_error(shared_from_this(), err);
    close();
}

void tcp_client::send_next() {
    assert(!send_queue_.empty());

    auto &resp_pair = send_queue_.front();
    resp_pair.first->update_header_buffer();

    if (!is_open()) {
        resp_pair.second(boost::asio::error::not_connected);
        send_queue_.pop_front();
        return;
    }

    std::array<boost::asio::const_buffer, 2> send_buffers {
        boost::asio::buffer(resp_pair.first->header_buffer()),
        boost::asio::buffer(resp_pair.first->body())};

    sock_.async_send(send_buffers,
                     [self = shared_from_this()](const auto &err, auto sent) {
                         self->send_queue_.front().second(err);
                         self->send_queue_.pop_front();

                         if (err) {
                             self->on_error(err);
                             return;
                         }

                         if (!self->send_queue_.empty()) {
                             self->send_next();
                         }
                     });
}

void tcp_client::enqueue_response(std::shared_ptr<response> resp,
                                  send_handler &&           cb) {
    send_queue_.push_back(std::make_pair(std::move(resp), std::move(cb)));

    if (!send_queue_.empty()) {
        send_next();
    }
}

void tcp_client::respond(std::shared_ptr<response> resp, send_handler &&cb) {
    io_.post([self = shared_from_this(), resp {std::move(resp)},
              cb {std::move(cb)}]() mutable {
        self->enqueue_response(std::move(resp), std::move(cb));
    });
}

void tcp_client::respond(const std::string &str,
                         std::uint32_t      code,
                         std::uint32_t      tag,
                         send_handler &&    cb) {
    auto resp_ptr = std::make_shared<ts::net::response>();

    resp_ptr->code(code);
    resp_ptr->flags(0x00U);
    resp_ptr->tag(tag);
    resp_ptr->body(str);

    respond(std::move(resp_ptr), std::move(cb));
}

void tcp_client::respond(const std::string &str,
                         std::uint32_t      tag,
                         send_handler &&    cb) {
    respond(str, 0x00u, tag, std::move(cb));
}

void tcp_client::respond(std::uint32_t  code,
                         std::uint32_t  tag,
                         send_handler &&cb) {
    respond({}, code, tag, std::move(cb));
}

void tcp_client::respond(response_code  code,
                         std::uint32_t  tag,
                         send_handler &&cb) {
    respond(static_cast<std::uint32_t>(code), tag, std::move(cb));
}

} // namespace ts::net
