#ifndef EZTIK_DATA_HOTSPOT_USER_HPP
#define EZTIK_DATA_HOTSPOT_USER_HPP

#include <cassert>
#include <string>

namespace eztik::data::models {

struct hotspot_user final {
    std::string name;
    std::string password;

    static constexpr auto api_path = "/ip/hotspot/user";

    template <typename TMap>
    static inline auto create(const TMap &m) -> hotspot_user {
        assert(m.contains("name"));
        assert(m.contains("password"));

        return {.name = m["name"], .password = m["password"]};
    }
};

} // namespace eztik::data::models

#endif
