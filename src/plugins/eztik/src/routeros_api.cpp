#include "eztik/routeros/api.hpp"

#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/read.hpp>
#include <boost/system/error_code.hpp>

#include <cassert>
#include <iostream>

using boost::asio::transfer_exactly;
using boost::asio::placeholders::bytes_transferred;
using boost::asio::placeholders::error;

namespace ip = boost::asio::ip;

namespace eztik::routeros {

void api::open(const std::string &    host,
               std::uint16_t          port,
               const connect_handler &cb) {
    try {
        ip::tcp::endpoint ep {ip::make_address(host), port};
        sock_.async_connect(ep, cb);
    } catch (...) {
        cb(boost::asio::error::bad_descriptor);
    }
}

void api::send(const request_sentence &s, const send_handler &cb) {
    assert(is_open());

    buffer_.clear();
    s.encode(buffer_);

    sock_.async_send(boost::asio::buffer(buffer_), cb);
}

void api::close() {
    if (sock_.is_open()) {
        sock_.close();
    }
    state(read_state::closed);
}

void api::start() {
    assert(sock_.is_open());
    assert(state() == read_state::idle);

    read_next_word();
}

void api::stop() {
    if (state() == read_state::closed) {
        return;
    }

    close();
}

void api::read_next(std::size_t n) {
    context().buffer().resize(n);

    boost::asio::async_read(
        sock_, boost::asio::buffer(context().buffer(), n), transfer_exactly(n),
        [this](const auto &err, auto nread) {
            handle_read(err, {context().buffer().c_str(), nread});
        });
}

void api::read_next_word() {
    if (state() == read_state::closed) {
        return;
    }

    assert(state() == read_state::idle);

    state(read_state::reading_length);
    read_next(1);
}

void api::on_reading_length(std::string_view data) {
    assert(state() == read_state::reading_length);
    assert(data.size() == 1);

    std::uint8_t fbyte {static_cast<std::uint8_t>(data[0])};
    std::uint8_t mbytes {};

    if (fbyte == 0) {
        handler_.on_response(context().current_sentence());
        context().current_sentence().clear();

        state(read_state::idle);
        read_next_word();
    } else if ((fbyte & 0x80) != 0x00) {
        if ((fbyte & 0xC0) == 0x80) {
            fbyte &= ~0xC0;
            mbytes = 1;
        } else if ((fbyte & 0xE0) == 0xC0) {
            fbyte &= ~0xE0;
            mbytes = 2;
        } else {
            if ((fbyte & 0xF0) == 0xE0) {
                fbyte &= ~0xF0;
            }
            mbytes = 3;
        }

        boost::asio::async_read(
            sock_, boost::asio::buffer(context().buffer(), mbytes),
            transfer_exactly(mbytes),
            [this, fbyte, mbytes](const auto &err, auto nread) {
                assert(nread == mbytes);
                if (err) {
                    on_error(err);
                    return;
                }

                std::size_t word_len {fbyte};

                for (std::size_t i {0}; i < mbytes; ++i) {
                    word_len = (word_len << 8) | (context().buffer()[i] & 0xFF);
                }

                state(read_state::reading_word);
                read_next(word_len);
            });

    } else {
        state(read_state::reading_word);
        read_next(fbyte);
    }
}
void api::on_reading_word(std::string_view data) {
    assert(state() == read_state::reading_word);

    context().current_sentence().push(std::string {data});
    state(read_state::idle);
    read_next_word();
}

void api::on_error(const boost::system::error_code &err) {
    handler_.on_error(err);
}

} // namespace eztik::routeros
