#include "app/app.hpp"

#include "log/core.hpp"
#include "log/logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

#include <stdexcept>
#include <iostream>

namespace ts::app {

void tiksrv_app::initialize() {
    logger_.info("Initializing application");
}

void tiksrv_app::run() {
    initialize();

    logger_.info("Running application");

    try {
        server_.start();
    } catch (const std::exception& ex) {
        logger_.error("Could not start server: {}", ex.what());
        throw std::runtime_error{"Could not start application"};
    }
}

} // namespace ts::app
