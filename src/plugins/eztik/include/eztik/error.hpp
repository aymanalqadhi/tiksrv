#ifndef EZTIK_ERROR_HPP
#define EZTIK_ERROR_HPP

#include <boost/system/error_code.hpp>

#include <iostream>
#include <string>

namespace eztik {

namespace detail {

class eztik_error_category : public boost::system::error_category {
  public:
    auto name() const noexcept -> const char * override final {
        return "eztik";
    }

    auto message(int c) const -> std::string override final;

    auto default_error_condition(int c) const noexcept
        -> boost::system::error_condition override final;
};

} // namespace detail

enum class error_code {
    success                   = 0,
    invalid_response          = 1,
    fatal_response            = 2,
    invalid_response_tag      = 3,
    invalid_login_credentials = 4,
};

auto eztik_error_category() -> const boost::system::error_category &;

auto make_error_code(eztik::error_code e) -> boost::system::error_code;

auto make_error_condition(eztik::error_code e) -> std::error_condition;

} // namespace eztik

namespace boost::system {

template <>
struct is_error_code_enum<eztik::error_code> : std::true_type {};

template <>
struct is_error_condition_enum<eztik::error_code> : std::true_type {};

} // namespace boost::system

#endif
