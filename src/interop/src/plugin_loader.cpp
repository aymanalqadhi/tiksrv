#include "interop/plugin.hpp"
#include "interop/plugin_loader.hpp"

#include <boost/dll/import.hpp>
#include <boost/dll/import_mangled.hpp>
#include <boost/dll/shared_library.hpp>

#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace ts::interop {

auto plugin_loader::load(const std::string &filename) -> plugin_wrapper {

    plugin_factory factory =
        boost::dll::import_alias<plugin_create_t>(filename, plugin_factory_sym);
    auto wrapper = plugin_wrapper {std::move(factory)};

    logger_.debug("Loaded plugin `{}'", filename);
    logger_.debug("Plugin name: {}", wrapper->name());
    logger_.debug("Plugin author: {}", wrapper->author());
    logger_.debug("Plugin version: {}", wrapper->version());

    return wrapper;
}

auto plugin_loader::load_all(const std::string &dirname)
    -> std::vector<plugin_wrapper> {
    std::vector<plugin_wrapper> ret {};

    if (!std::filesystem::exists(dirname) ||
        !std::filesystem::is_directory(dirname)) {
        throw std::runtime_error {"Invalid plugins path"};
    }

    for (const auto &entry : std::filesystem::directory_iterator {dirname}) {
        if (!entry.is_regular_file() || entry.path().extension() != ".so") {
            continue;
        }

        try {
            ret.push_back(load(entry.path()));
        } catch (const std::exception &ex) {
            logger_.warn("Could not load plugin `{}': {}",
                         entry.path().string(), ex.what());
        }
    }

    return ret;
}

} // namespace ts::interop
