#ifndef TIKSRV_CONFIG_CONFIG_HPP
#define TIKSRV_CONFIG_CONFIG_HPP

#include <boost/program_options/options_description.hpp>
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
  public:
    config();

    inline bool has(const std::string &key) const {
        return values_.count(key) > 0;
    }

    inline auto operator[](const std::string &key) const
        -> const boost::program_options::variable_value & {
        return values_[key];
    }

    inline void operator>>(std::ostream &os) {
        os << desc_ << std::endl;
    }

    inline auto begin() const noexcept {
        return values_.begin();
    }

    inline auto end() const noexcept {
        return values_.end();
    }

    auto has(config_key key) const -> bool;
    const boost::program_options::variable_value &
         operator[](config_key key) const;
    void parse_argv(int argc, const char *const argv[]);
    void parse_config_file(std::string_view                             path,
                           boost::program_options::options_description &desc);

    inline void parse_config_file(std::string_view path) {
        parse_config_file(path, desc_);
    }

    static auto from_argv(int argc, const char *const argv[]) -> config;

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
    boost::program_options::variables_map       values_;
    boost::program_options::options_description desc_;
};

} // namespace ts::config

#endif
