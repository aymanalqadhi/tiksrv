#ifndef EZTIK_ERROR_HPP
#define EZTIK_ERROR_HPP

#include <boost/system/error_code.hpp>
#include <iostream>
#include <string>

namespace eztik {
enum class error_code { success = 0 };

}
namespace boost ::system {

template <>
struct is_error_code_enum<eztik::error_code> : std::true_type {};

} // namespace boost::system

namespace detail {

class eztik_error_category : public boost::system::error_category {
  public:
    auto name() const noexcept -> const char * override final {
        return "ezTik Error";
    }

    auto message(int c) const -> std::string override final;

    auto default_error_condition(int c) const noexcept
        -> boost::system::error_condition override final;
};

} // namespace detail

extern inline const detail::eztik_error_category &ConversionErrc_category() {
    static detail::eztik_error_category c;
    return c;
}

inline auto make_error_code(eztik::error_code e) -> boost::system::error_code {
    return {static_cast<int>(e), detail::eztik_error_category()};
}

#endif
