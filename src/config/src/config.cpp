#include "config/config.hpp"

#include <array>
#include <string>

namespace po = boost::program_options;

namespace {

constexpr std::array config_string_keys {"port",   "backlog", "plugins",
                                         "config", "help",    "version"};

} // namespace

namespace ts::config {

config::config() : desc_ {"Allowed Options"} {
    desc_.add_options()(
        "port,p",
        po::value<std::uint16_t>()->default_value(defaults::listen_port),
        "The server listening port");
    desc_.add_options()(
        "backlog,b",
        po::value<std::uint32_t>()->default_value(defaults::liten_backlog),
        "The maximum number in the listening queue");
    desc_.add_options()(
        "plugins",
        po::value<std::string>()->default_value(defaults::plugins_path),
        "The path from which to load the application plugins");
    desc_.add_options()(
        "config,c",
        po::value<std::string>()->default_value(defaults::config_file),
        "The configuration file location");
    desc_.add_options()("help,h", "Show help message");
    desc_.add_options()("version,V", "Show app version");
}


auto config::has(config_key key) const -> bool {
    return has(::config_string_keys[static_cast<std::uint32_t>(key)]);
}

auto config::operator[](config_key key) const -> const po::variable_value & {
    return operator[](::config_string_keys[static_cast<std::uint32_t>(key)]);
}

} // namespace ts::config
