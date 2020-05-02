#ifndef TIKSRV_SERVICES_HOOKS_MANAGER_HPP
#define TIKSRV_SERVICES_HOOKS_MANAGER_HPP

#include "services/service.hpp"

#include "log/logger.hpp"

#include <any>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace ts::services {

enum class hooks_group { connection, disconnection };

class hooks_manager final : public service {
    using hook = std::function<void(std::any)>;

  public:
    static constexpr auto name = "hooks_manager";

    hooks_manager() : name_ {"Hooks Manager"}, logger_ {"hooks_manager"} {
    }

    auto display_name() const noexcept -> const std::string & override {
        return name_;
    }

    auto is_required() const noexcept -> bool override {
        return true;
    }

    inline auto has_group(std::uint32_t group) -> bool {
        return groups_.contains(group);
    }

    void initialize() override;

    void create_group(std::uint32_t group);

    void register_hook(std::uint32_t group, hook h);

    void run_hooks(std::uint32_t group, std::any data);

    inline void create_group(hooks_group group) {
        create_group(static_cast<std::uint32_t>(group));
    }

    inline void register_hook(hooks_group group, hook h) {
        register_hook(static_cast<std::uint32_t>(group), std::move(h));
    }

    inline void run_hooks(hooks_group group, std::any data) {
        run_hooks(static_cast<std::uint32_t>(group), std::move(data));
    }

  private:
    const std::string name_;
    ts::log::logger   logger_;

    std::map<std::uint32_t, std::vector<hook>> groups_;
};

} // namespace ts::services

#endif
