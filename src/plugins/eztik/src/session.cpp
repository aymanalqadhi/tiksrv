#include "eztik/routeros/sentence.hpp"
#include "eztik/session.hpp"

#include <boost/asio/error.hpp>

#include <iostream>

namespace eztik {

void session::on_error(const boost::system::error_code &err) {
    if (err == boost::asio::error::eof) {
        logger_.info("Session #{} API connection was closed", id_);
    } else {
        logger_.error("Session #{} API connection encountered an error: {}",
                      id_, err.message());
    }

    handler_.on_close(*this);
}

} // namespace eztik
