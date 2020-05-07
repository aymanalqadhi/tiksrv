#include "eztik/error.hpp"

#include <string>

namespace detail {

auto eztik_error_category::message(int c) const -> std::string {
    switch (static_cast<eztik::error_code>(c)) {
    case eztik::error_code::success:
        return "Success";
    default:
        return "Unknoww Error";
    }
}

boost::system::error_condition
eztik_error_category::default_error_condition(int c) const noexcept {
    switch (static_cast<eztik::error_code>(c)) {
    case eztik::error_code::success:
        return make_error_condition(boost::system::errc::success);
    default:
        return boost::system::error_condition(c, *this);
    }
}
} // namespace detail
