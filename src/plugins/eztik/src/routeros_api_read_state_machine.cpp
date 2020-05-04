#include "eztik/routeros/api_read_state_machine.hpp"

#include <boost/asio/error.hpp>

#include <string_view>

namespace eztik::routeros {

void api_read_state_machine::handle_read(const boost::system::error_code &err,
                                         std::string_view                 buf) {
    if (state_ == read_state::closed) {
        return;
    }

    if (err) {
        on_error(err);
        return;
    }

    if (buf.size() == 0) {
        on_error(boost::asio::error::eof);
        return;
    }

    switch (state_) {
    case read_state::reading_length:
        on_reading_length(std::move(buf));
        break;

    case read_state::reading_word:
        on_reading_word(std::move(buf));
        break;

    default:
        break;
    }
}

} // namespace eztik::routeros
