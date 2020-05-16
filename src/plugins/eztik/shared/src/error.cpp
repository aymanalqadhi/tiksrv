#include "eztik/error.hpp"

#include <boost/system/error_code.hpp>

#include <string>

namespace {

eztik::detail::eztik_error_category category {};

}
namespace eztik {

namespace detail {

auto eztik_error_category::message(int c) const -> std::string {
    switch (static_cast<eztik::error_code>(c)) {
    case eztik::error_code::success:
        return "Success";
    case eztik::error_code::invalid_response:
        return "Invalid response";
    case eztik::error_code::fatal_response:
        return "Fatal response";
    case eztik::error_code::untagged_response:
        return "Untagged response";
    case eztik::error_code::invalid_response_tag:
        return "Invalid response tag";
    case eztik::error_code::invalid_login_credentials:
        return "Invalid login credentails";
    default:
        return "Unknoww error";
    }
}

} // namespace detail

auto detail::eztik_error_category::default_error_condition(int c) const noexcept
    -> boost::system::error_condition {
    switch (static_cast<eztik::error_code>(c)) {
    case eztik::error_code::success:
        return make_error_condition(boost::system::errc::success);
    case eztik::error_code::invalid_response:
    case eztik::error_code::fatal_response:
        return make_error_condition(boost::system::errc::invalid_argument);
    default:
        return boost::system::error_condition(c, *this);
    }
}

auto eztik_error_category() -> const boost::system::error_category & {
    return category;
}

auto make_error_code(eztik::error_code e) -> boost::system::error_code {
    return {static_cast<int>(e), ::category};
}

auto make_error_condition(eztik::error_code e) -> std::error_condition {
    return std::error_condition(static_cast<int>(e), ::category);
}

} // namespace eztik
