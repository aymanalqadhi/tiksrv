#include "services/hooks_manager.hpp"

#include <any>
#include <assert.h>
#include <cstdint>
#include <vector>

namespace ts::services {

void hooks_manager::initialize() {
    create_group(hooks_group::connection);
    create_group(hooks_group::disconnection);
}

void hooks_manager::create_group(std::uint32_t group) {
    assert(!groups_.contains(group));

    logger_.debug("Creating hooks group #{}", group);
    groups_.emplace(std::make_pair(group, std::vector<hook>{}));
}

void hooks_manager::register_hook(std::uint32_t group, hook h) {
    if (!groups_.contains(group)) {
        create_group(group);
    }

    logger_.debug("Registering a new hook for hooks group #{}", group);
    groups_[group].push_back(std::move(h));
}

void hooks_manager::run_hooks(std::uint32_t group, std::any data) {
    assert(groups_.contains(group));

    logger_.debug("Running hooks group #{}", group);

    for (const auto &hook : groups_[group]) {
        hook(std::move(data));
    }
}

} // namespace ts::services
