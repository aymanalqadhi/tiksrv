#ifndef EZTIK_ROUTEROS_UTIL_HPP
#define EZTIK_ROUTEROS_UTIL_HPP

#include <string>

namespace eztik::routeros {

constexpr auto md5_bits = 128U;
constexpr auto md5_size = md5_bits / 8U;

std::string hash_password(const std::string &plain, const std::string &cha);

} // namespace eztik::routeros

#endif
