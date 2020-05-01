#ifndef TIKSRV_INTEROP_PLUGIN_HPP
#define TIKSRV_INTEROP_PLUGIN_HPP

#include "interop/command.hpp"

#include "services/services_manager.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ts::interop {

class plugin {
  public:
    using export_func = std::function<void(
        std::uint16_t, std::uint16_t, std::unique_ptr<command>)>;

    virtual ~plugin() {
    }

    virtual auto name() const noexcept -> const std::string &    = 0;
    virtual auto author() const noexcept -> const std::string &  = 0;
    virtual auto version() const noexcept -> const std::string & = 0;

    virtual void initialize(ts::services::services_manager &svcs) noexcept = 0;
    virtual void export_commands(export_func export_cb) const noexcept     = 0;
};

} // namespace ts::interop

#endif
