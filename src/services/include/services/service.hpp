#ifndef TIKSRV_SERVICES_SERVICE_HPP
#define TIKSRV_SERVICES_SERVICE_HPP

#include <map>
#include <string>
#include <type_traits>

#define TS_MAKE_SERVICE(type) static constexpr auto name = #type

namespace ts::services {

class service {
  public:
    [[nodiscard]] virtual auto display_name() const noexcept
        -> const std::string &                                      = 0;
    [[nodiscard]] virtual auto is_required() const noexcept -> bool = 0;

    virtual void initialize() = 0;
};

template <typename, typename = std::void_t<>>
struct has_name_member : std::false_type {};

template <typename T>
struct has_name_member<T, std::void_t<typename T::name>> : std::true_type {};

template <typename T>
struct is_service {
    static constexpr auto value =
        std::is_base_of<service, T>::value && has_name_member<T>::value;
};

} // namespace ts::services

#endif
