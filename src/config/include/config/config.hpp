#ifndef TIKSRV_CONFIG_CONFIG_HPP
#define TIKSRV_CONFIG_CONFIG_HPP

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

namespace ts::config {

enum class config_key : std::uint32_t {
    listen_port,
    liten_backlog,
    plugins_path,
    config_file,
    help,
    version
};

class config final {
    using options_description = boost::program_options::options_description;
    using variables_map       = boost::program_options::variables_map;

  public:
    config();

    [[nodiscard]] inline auto has(const std::string &key) const noexcept
        -> bool {
        return values_.count(key) > 0;
    }

    inline auto operator[](const std::string &key) const
        -> const boost::program_options::variable_value & {
        return values_[key];
    }

    inline void operator>>(std::ostream &os) {
        os << desc_ << std::endl;
    }

    [[nodiscard]] inline auto begin() const noexcept
        -> variables_map::const_iterator {
        return values_.begin();
    }

    [[nodiscard]] inline auto end() const noexcept
        -> variables_map::const_iterator {
        return values_.end();
    }

    [[nodiscard]] auto has(config_key key) const -> bool;

    [[nodiscard]] auto operator[](config_key key) const
        -> const boost::program_options::variable_value &;

    inline void parse_argv(const std::vector<const char *> &args) {
        boost::program_options::store(
            boost::program_options::parse_command_line(args.size(), args.data(),
                                                       desc_),
            values_);
        boost::program_options::notify(values_);
    }

    inline void parse_config_file(std::string_view     path,
                                  options_description &desc) {
        boost::program_options::store(
            boost::program_options::parse_config_file(path.data(), desc, true),
            values_);
    }

    inline void parse_config_file(std::string_view path) {
        parse_config_file(path, desc_);
    }

    [[nodiscard]] inline static auto
    from_argv(const std::vector<const char *> &args) -> config {
        config ret {};
        ret.parse_argv(args);
        return ret;
    }

    struct defaults {
        static constexpr auto listen_port   = 3434u;
        static constexpr auto liten_backlog = 100u;
        static constexpr auto plugins_path  = "plugins";
        static constexpr auto config_file   = "tiksrv.cfg";
    };

    struct constants {
        static constexpr auto app_name    = "tiksrv";
        static constexpr auto app_version = "0.1";
    };

  private:
    variables_map       values_;
    options_description desc_;
};

} // namespace ts::config

#endif
