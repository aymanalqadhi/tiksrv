#include "eztik/routeros/sentence.hpp"
#include "eztik/session.hpp"

#include <boost/asio/error.hpp>

#include <iostream>

namespace eztik {

void session::on_error(const boost::system::error_code &err) {
    if (err != boost::asio::error::eof) {
        logger_.error("Session #{} API connection encountered an error: {}",
                      id_, err.message());
    }

    on_close();
}

void session::on_close() {
    logger_.info("Session #{} API connection was closed", id_);

    if (api_.is_open()) {
        api_.stop();
    }

    handler_.on_close(*this);
}

void session::on_response(const eztik::routeros::sentence &sen) {
    try {
        if (!eztik::routeros::response_sentence::is_valid_response(sen)) {
            throw std::runtime_error {"Invalid response"};
        }

        eztik::routeros::response_sentence resp {sen};

        if (!resp.is_tagged()) {
            logger_.warn("Session #{} is discarding an untagged response", id_);
            return;
        }

        if (!has_read_callback(resp.tag())) {
            logger_.warn("Session #{} is discarding a response with tag #{} "
                         "with no associated callback",
                         id_, resp.tag());
            return;
        }

        auto cb_itr = read_cbs_.find(resp.tag());
        assert(cb_itr != read_cbs_.end());

        cb_itr->second({}, *this, std::move(resp));

        if (!cb_itr->second.is_permanent()) {
            read_cbs_.erase(cb_itr);
        }

    } catch (const std::exception &ex) {
        logger_.error("Response error: {}", ex.what());
        on_close();
    }
}

void session::send(const eztik::routeros::request_sentence &sen,
                   session_read_callback::callback &&       cb,
                   bool                                     permanent) {
    api_.send(sen, [this, tag = sen.tag(), permanent, cb](const auto &err,
                                                          auto sent) mutable {
        if (err) {
            cb(err, *this, {});
            on_close();
            return;
        }

        add_read_callback(tag, std::move(cb), permanent);
    });
}

void session::add_read_callback(std::uint32_t                     tag,
                                session_read_callback::callback &&cb,
                                bool                              permanent) {
    assert(!read_cbs_.contains(tag));

    read_cbs_.emplace(
        std::make_pair(tag, session_read_callback {std::move(cb), permanent}));
}

} // namespace eztik
