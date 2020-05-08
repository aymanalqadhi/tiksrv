#include "eztik/routeros/api.hpp"
#include "eztik/routeros/util.hpp"

#include "eztik/error.hpp"

#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

#include <cassert>
#include <iostream>

using boost::asio::transfer_exactly;

namespace ip = boost::asio::ip;

namespace eztik::routeros {

void api::open(const std::string &host,
               std::uint16_t      port,
               connect_handler && cb) {
    try {
        ip::tcp::endpoint ep {ip::make_address(host), port};
        sock_.async_connect(ep, [this, cb {std::move(cb)}](const auto &err) {
            if (!err) {
                state(read_state::idle);
                start();
            }

            cb(err);
        });
    } catch (...) {
        cb(boost::asio::error::address_family_not_supported);
    }
}

void api::close() {
    assert(state() != read_state::closed);

    if (sock_.is_open()) {
        sock_.close();
    }

    state(read_state::closed);
}

void api::send(const eztik::routeros::request_sentence &sen,
               api_read_callback::callback &&           cb,
               bool                                     permanent) {
    assert(is_ready());

    auto buf = std::make_shared<std::vector<std::uint8_t>>();
    sen.encode(*buf);

    sock_.async_send(
        boost::asio::buffer(*buf),
        [this, cb {std::move(cb)}, buf, tag = sen.tag(),
         permanent](const auto &err, const auto &sent) mutable {
            if (err) {
                cb(err, *this, {});
                close();
            } else {
                read_cbs_.emplace(std::make_pair(
                    tag, api_read_callback(std::move(cb), permanent)));
            }
        });
}

void api::login(const std::string &username,
                const std::string &password,
                login_handler &&   cb) {
    auto req = make_request("/login");

    send(req, [this, username, password,
               cb {std::move(cb)}](const auto &err, auto &api, auto &&resp) {
        if (err) {
            logger_.error("Session #{} API connection login error: {}", id_,
                          err.message());
            cb(false);
            return;
        }

        if (resp.type() != response_sentence_type::normal || !resp.has("ret") ||
            resp["ret"].size() != md5_size * 2) {
            logger_.error(
                "Invalid login response for session #{} API connection", id_);
            cb(false);
            return;
        }

        auto req = make_request("/login");
        req.push_param("name", username);
        req.push_param("response", "00{}",
                       hash_password(password, resp["ret"]));

        send(req, [this, cb {std::move(cb)}](const auto &err, auto &api,
                                             auto &&resp) {
            if (err) {
                logger_.error("Session #{} API connection login error: {}", id_,
                              err.message());
                cb(false);
                return;
            }

            if (resp.type() != response_sentence_type::normal) {
                logger_.error(
                    "API connection of session #{} was unable to login: {}",
                    id_, resp["message"]);
                cb(false);
                return;
            }

            cb(true);
        });
    });
}

void api::start() {
    assert(sock_.is_open());
    assert(state() == read_state::idle);

    read_next_word();
}

void api::read_next(std::size_t n) {
    if (!is_ready()) {
        return;
    }

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
        handle_response(context().current_sentence());
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
        } else if ((fbyte & 0xF0) == 0xE0) {
            fbyte &= ~0xF0;
            mbytes = 3;
        } else if (fbyte == 0xF0) {
            mbytes = 4;
        } else {
            on_error(boost::asio::error::message_size);
            return;
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
    close();
    handler_.on_error(err);
}

void api::handle_response(const sentence &s) {
    try {
        if (!eztik::routeros::response_sentence::is_valid_response(s)) {
            on_error(eztik::error_code::invalid_response);
            return;
        }

        eztik::routeros::response_sentence resp {s};

        if (!resp.is_tagged()) {
            logger_.warn("API connection for session #{} is discarding an "
                         "untagged response",
                         id_);
            return;
        }

        if (!read_cbs_.contains(resp.tag())) {
            on_error(eztik::error_code::invalid_response_tag);
            return;
        }

        auto cb_itr = read_cbs_.find(resp.tag());
        cb_itr->second({}, *this, std::move(resp));

        if (!cb_itr->second.is_permanent()) {
            read_cbs_.erase(cb_itr);
        }

    } catch (const std::exception &ex) {
        on_error(eztik::error_code::fatal_response);
    }
}

} // namespace eztik::routeros
