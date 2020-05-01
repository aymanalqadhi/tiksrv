#ifndef TIKSRV_SERVICES_SERVICE_HPP
#define TIKSRV_SERVICES_SERVICE_HPP

#include <map>
#include <string>
#include <type_traits>

#define TS_MAKE_SERVICE(type) static constexpr auto name = #type

namespace ts::services {

class service {
  public:
    virtual auto display_name() const noexcept -> const std::string & = 0;
    virtual auto is_required() const noexcept -> bool                 = 0;

    virtual void initialize() = 0;
};

template <typename T>
class is_service {
    using one = char;

    struct two {
        char x[2];
    };

    template <typename C>
    static constexpr one test(typeof(&C::name));
    template <typename C>
    static constexpr two test(...);

  public:
    static constexpr auto value = std::is_base_of<service, T>::value &&
                                  sizeof(test<T>(0)) == sizeof(char);
};

} // namespace ts::services

#endif
