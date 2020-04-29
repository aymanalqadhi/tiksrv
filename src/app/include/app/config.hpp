#ifndef TIKSRV_APP_CONFIG_HPP
#define TIKSRV_APP_CONFIG_HPP

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

namespace ts::app {

enum class config_key : std::uint32_t {
    listen_port,
    liten_backlog,
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

    inline const boost::program_options::variable_value &
    operator[](const std::string &key) const {
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

    bool has(config_key key) const;
    const boost::program_options::variable_value &
         operator[](config_key key) const;
    void parse_argv(int argc, const char *const argv[]);
    void parse_config_file(std::string_view path);

    static config from_argv(int argc, const char *const argv[]);

    struct defaults {
        static constexpr auto listen_port   = 3434u;
        static constexpr auto liten_backlog = 100u;
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

} // namespace ts::app

#endif
