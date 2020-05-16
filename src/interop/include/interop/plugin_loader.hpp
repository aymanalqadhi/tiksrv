#ifndef TIKSRV_INTEROP_PLUGIN_LOADER_HPP
#define TIKSRV_INTEROP_PLUGIN_LOADER_HPP

#include "interop/plugin.hpp"
#include "log/logger.hpp"

#include <boost/dll/shared_library.hpp>
#include <boost/function.hpp>

#include <functional>
#include <vector>

namespace ts::interop {

constexpr auto plugin_factory_sym = "create_plugin";

typedef std::unique_ptr<plugin>(plugin_create_t)(
    ts::services::services_manager &);
using plugin_factory = std::function<plugin_create_t>;

struct plugin_wrapper final {
    plugin_wrapper(std::unique_ptr<plugin> &&   plug,
                   boost::dll::shared_library &&handle)
        : plugin_ {std::move(plug)}, handle_ {std::move(handle)} {
    }

    auto operator->() -> plugin * {
        return plugin_.get();
    }

  private:
    std::unique_ptr<plugin>    plugin_;
    boost::dll::shared_library handle_;
};

class plugin_loader final {
  public:
    plugin_loader() : logger_ {"plugin_loader"} {
    }

    auto load(const std::string &filename, ts::services::services_manager &svcs)
        -> plugin_wrapper;
    auto load_all(const std::string &             dirname,
                  ts::services::services_manager &svcs)
        -> std::vector<plugin_wrapper>;

  private:
    ts::log::logger logger_;
};

} // namespace ts::interop

#endif
