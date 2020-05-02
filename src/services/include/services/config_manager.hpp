#ifndef TIKSRV_SERVICES_CONFIG_MANAGER_HPP
#define TIKSRV_SERVICES_CONFIG_MANAGER_HPP

#include "services/service.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/value_semantic.hpp>

#include <string>

namespace ts::services {

class config_manager : public service {
  public:
    static constexpr auto name = "config_manager";

    config_manager() : name_ {"Configuration Manager"} {
    }

    auto display_name() const noexcept -> const std::string & override {
        return name_;
    }

    auto is_required() const noexcept -> bool override {
        return true;
    }

    void initialize() override {
    }

    template <typename T>
    inline void add_option(const std::string &opt) {
        desc_.add_options(opt,
                          boost::program_options::value<T>()->required(false));
    }

    template <typename T>
    inline void add_option(const std::string &opt, const T &def) {
        desc_.add_options(
            opt,
            boost::program_options::value<T>()->required(false)->default_value(
                def));
    }

    inline auto options() noexcept
        -> boost::program_options::options_description & {
        return desc_;
    }

  private:
    const std::string                           name_;
    boost::program_options::options_description desc_;
};

} // namespace ts::services

#endif
