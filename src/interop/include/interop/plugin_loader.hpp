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

typedef std::unique_ptr<plugin>(plugin_create_t)();
using plugin_factory = std::function<plugin_create_t>;

struct plugin_wrapper final {
    plugin_wrapper(plugin_factory &&factory) : factory_ {std::move(factory)} {
        if ((plugin_ = factory_()) == nullptr) {
            throw std::runtime_error {"Invalid plugin: factory returned null"};
        }
    }

    auto operator->() -> plugin * {
        return plugin_.get();
    }

    auto operator()() {
        return factory_();
    }

  private:
    plugin_factory          factory_;
    std::unique_ptr<plugin> plugin_;
};

class plugin_loader final {
  public:
    plugin_loader() : logger_ {"plugin_loader"} {
    }

    auto load(const std::string &filename) -> plugin_wrapper;
    auto load_all(const std::string &dirname) -> std::vector<plugin_wrapper>;

  private:
    ts::log::logger logger_;
};

} // namespace ts::interop

#endif
