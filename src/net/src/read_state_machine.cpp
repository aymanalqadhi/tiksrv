#include "net/read_state_machine.hpp"

#include <boost/asio/error.hpp>

#include <string_view>

namespace ts::net {

void read_state_machine::handle_read(std::size_t                      nread,
                                     std::string_view                 buf,
                                     const boost::system::error_code &err) {
    if (state_ == read_state::closed) {
        return;
    }

    if (err) {
        on_error(err);
        return;
    }

    if (nread == 0) {
        on_error(boost::asio::error::eof);
        return;
    }

    switch (state_) {
    case read_state::reading_header:
        on_reading_header(std::move(buf));
        break;

    case read_state::reading_body:
        on_reading_body(std::move(buf));
        break;

    default:
        break;
    }
}

} // namespace ts::net
