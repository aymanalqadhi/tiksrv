#include "eztik/routeros/api.hpp"
#include "eztik/routeros/commands.hpp"

#include "eztik/error.hpp"

#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

#include <cassert>

using boost::asio::transfer_exactly;

namespace ip       = boost::asio::ip;
namespace commands = eztik::routeros::commands;

namespace eztik::routeros {

void api::open(const std::string &host,
               std::uint16_t      port,
               connect_handler && cb) {
    boost::system::error_code ec {};

    auto address = ip::address::from_string(host, ec);

    if (ec) {
        cb(ec);
        return;
    }

    ip::tcp::endpoint ep {std::move(address), port};
    sock_.async_connect(
        ep, [self = shared_from_this(), cb {std::move(cb)}](const auto &err) {
            if (!err) {
                self->state(read_state::idle);
                self->start();
            }

            cb(err);
        });
}

void api::close() {
    assert(state() != read_state::closed);

    if (sock_.is_open()) {
        sock_.close();
    }

    state(read_state::closed);
}

void api::send(std::shared_ptr<request_sentence> req,
               api_read_callback::callback &&    cb,
               bool                              permanent) {
    io_.post([self = shared_from_this(), req {std::move(req)},
              cb {std::move(cb)}, permanent]() mutable {
        self->send_queue_.push_back(std::make_pair(
            std::move(req), api_read_callback {std::move(cb), permanent}));
        self->send_next();
    });
}

void api::login(const std::string &username,
                const std::string &password,
                login_handler &&   cb) {
    auto req = make_command<commands::login1>();

    send(std::move(req), [this, username, password, cb {std::move(cb)}](
                             const auto &err, auto &api, auto &&resp) {
        if (err) {
            cb(err);
            return;
        }

        if (resp.type() != response_sentence_type::normal ||
            !resp.has(commands::login2::challenge_param) ||
            resp[commands::login2::challenge_param].size() != md5_size * 2) {
            cb(eztik::error_code::invalid_response);
            return;
        }

        auto req = make_command<commands::login2>(
            std::move(username), password,
            resp[commands::login2::challenge_param]);

        send(std::move(req), [this, cb {std::move(cb)}](
                                 const auto &err, auto &api, auto &&resp) {
            if (err) {
                cb(err);
                return;
            }

            if (resp.type() != response_sentence_type::normal) {
                cb(eztik::error_code::invalid_login_credentials);
                return;
            }

            logged_in_ = true;
            cb(eztik::error_code::success);
        });
    });
}

void api::on_reading_length(std::string_view data) {
    assert(is_open());
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
            [self = shared_from_this(), fbyte, mbytes](const auto &err,
                                                       auto        nread) {
                if (!self->is_open()) {
                    return;
                }

                if (err) {
                    self->on_error(err);
                    return;
                }

                assert(nread == mbytes);
                std::size_t word_len {fbyte};

                for (std::size_t i {0}; i < mbytes; ++i) {
                    word_len =
                        (word_len << 8) | (self->context().buffer()[i] & 0xFF);
                }

                self->state(read_state::reading_word);
                self->read_next(word_len);
            });

    } else {
        state(read_state::reading_word);
        read_next(fbyte);
    }
}

void api::on_reading_word(std::string_view data) {
    assert(is_open());
    assert(state() == read_state::reading_word);

    context().current_sentence().push(std::string {data});
    state(read_state::idle);
    read_next_word();
}

void api::on_error(const boost::system::error_code &err) {
    handler_.on_error(err);

    if (is_open()) {
        close();
    }
}

void api::start() {
    assert(sock_.is_open());
    assert(state() == read_state::idle);

    read_next_word();
}

void api::read_next(std::size_t n) {
    context().buffer().resize(n);

    boost::asio::async_read(
        sock_, boost::asio::buffer(context().buffer(), n), transfer_exactly(n),
        [self = shared_from_this()](const auto &err, auto nread) {
            if (self->is_open()) {
                self->handle_read(err,
                                  {self->context().buffer().c_str(), nread});
            }
        });
}

void api::read_next_word() {
    if (!is_open()) {
        return;
    }

    assert(state() == read_state::idle);
    state(read_state::reading_length);
    read_next(1);
}

void api::handle_response(const sentence &s) {
    assert(is_open());

    try {
        if (!eztik::routeros::response_sentence::is_valid_response(s)) {
            on_error(eztik::error_code::invalid_response);
            return;
        }

        if (s.size() <= 1) {
            return; // empty response
        }

        eztik::routeros::response_sentence resp {s};

        if (!resp.is_tagged()) {
            on_error(eztik::error_code::untagged_response);
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

void api::send_next() {
    assert(!send_queue_.empty());

    auto [req, cb] = send_queue_.front();
    send_queue_.pop_front();

    if (!is_open()) {
        cb(boost::asio::error::not_connected, *this, {});
        return;
    }

    auto buf = std::make_shared<std::vector<std::uint8_t>>();
    req->encode(*buf);

    sock_.async_send(
        boost::asio::buffer(*buf),
        [self = shared_from_this(), buf, req {std::move(req)},
         cb(std::move(cb))](const auto &err, const auto &sent) mutable {
            if (!self->is_open()) {
                cb(boost::asio::error::not_connected, *self, {});
                return;
            }

            if (err) {
                cb(err, *self, {});
                self->close();
            } else {
                self->read_cbs_.emplace(
                    std::make_pair(req->tag(), std::move(cb)));
            }

            if (!self->send_queue_.empty()) {
                self->send_next();
            }
        });
}

} // namespace eztik::routeros
