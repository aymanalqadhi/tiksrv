#include "services/services_manager.hpp"

#include "fmt/format.h"

#include <stdexcept>

namespace ts::services {

void services_manager::initialize() {
    if (state_.load() != services_manager_state::uninitialized) {
        throw std::runtime_error {"Service have been already initialized"};
    }

    state_.store(services_manager_state::initialized);

    for (const auto &svc : services_) {
        try {
            logger_.debug("Initializing service `{} ({})'",
                          svc.second->display_name(), svc.first);
        } catch (const std::exception &ex) {
            if (svc.second->is_required()) {
                throw std::runtime_error {
                    fmt::format("Could not initialize required service `{}'",
                                svc.second->display_name())};
            }

            logger_.warn("Could not initialize optioal service `{}', the "
                         "service will not be available",
                         svc.second->display_name());
            services_.erase(svc.first);
        }
    }
}

} // namespace ts::services
