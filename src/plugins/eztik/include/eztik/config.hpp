#ifndef EZTIK_CONFIG_HPP
#define EZTIK_CONFIG_HPP

namespace eztik {

struct config_keys {
    static constexpr auto ros_ip           = "eztik.ros_ip";
    static constexpr auto ros_api_port     = "eztik.ros_api_port";
    static constexpr auto ros_api_user     = "eztik.ros_user";
    static constexpr auto ros_api_password = "eztik.ros_password";
};

struct config_defaults {
    static constexpr auto ros_ip           = "127.0.0.1";
    static constexpr auto ros_api_port     = 8728U;
    static constexpr auto ros_api_user     = "admin";
    static constexpr auto ros_api_password = "";
};

} // namespace eztik

#endif
