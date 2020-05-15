#ifndef EZTIK_ROUTEROS_UTIL_HPP
#define EZTIK_ROUTEROS_UTIL_HPP

#include <string>

namespace eztik::routeros {

std::string hash_password(const std::string &plain, const std::string &cha);

} // namespace eztik::routeros

#endif
