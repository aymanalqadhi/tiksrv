#ifndef EZTIK_ROUTEROS_UTIL_HPP
#define EZTIK_ROUTEROS_UTIL_HPP

#include <string>

namespace eztik::routeros {

auto hash_password(const std::string &plain, const std::string &cha)
    -> std::string;

} // namespace eztik::routeros

#endif
