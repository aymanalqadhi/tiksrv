#ifndef EZTIK_RESPONSE_HPP
#define EZTIK_RESPONSE_HPP

#include <cstdint>

namespace eztik {

constexpr std::uint16_t response_category = 0x0001;

enum class response_code : std::uint16_t {
    ros_session_open_failed  = 0x0000,
    ros_session_close_failed = 0x0001,
    ros_session_already_open = 0x0002,
    ros_session_not_open     = 0x0003,
    ros_session_closed       = 0x0004
};

[[nodiscard]] constexpr auto make_response_code(response_code code)
    -> std::uint32_t {
    return (response_category << 16) |
           (static_cast<std::uint16_t>(code) & 0xFFFF);
}

} // namespace eztik

#endif
