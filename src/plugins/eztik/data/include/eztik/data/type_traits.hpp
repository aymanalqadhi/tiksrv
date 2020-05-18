#ifndef EZTIK_DATA_TYPE_TRAITS_HPP
#define EZTIK_DATA_TYPE_TRAITS_HPP

#include <type_traits>

#include <string>
#include <unordered_map>

namespace eztik::data {

namespace detail {

using hashmap_t = std::unordered_map<std::string, std::string>;

}

template <typename T, typename = void>
struct has_api_path : std::false_type {};

template <typename T>
struct has_api_path<T, std::void_t<decltype(T::api_path)>> : std::true_type {};

template <typename T, typename = void>
struct has_create_function : std::false_type {};

template <typename T>
struct has_create_function<
    T,
    std::enable_if_t<std::is_same_v<std::decay_t<T>,
                                    decltype(T::template create(
                                        std::declval<detail::hashmap_t>()))>>>
    : std::true_type {};

template <typename T, typename = void>
struct is_api_model : std::false_type {};

template <typename T>
struct is_api_model<T, std::enable_if_t<has_create_function<T>::value>>
    : std::true_type {};

} // namespace eztik::data

#endif
