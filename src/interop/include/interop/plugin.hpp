#ifndef TIKSRV_INTEROP_PLUGIN_HPP
#define TIKSRV_INTEROP_PLUGIN_HPP

#include "interop/command.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ts::interop {

class plugin {
  public:
    virtual ~plugin() {
    }

    virtual auto name() const noexcept -> const std::string &    = 0;
    virtual auto author() const noexcept -> const std::string &  = 0;
    virtual auto version() const noexcept -> const std::string & = 0;

    virtual void export_commands(
        std::function<void(std::uint32_t, std::unique_ptr<command>)> export_cb)
        const noexcept = 0;
};

} // namespace ts::interop

#endif
