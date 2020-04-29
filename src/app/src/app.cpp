#include "app/app.hpp"

#include "log/core.hpp"
#include "log/logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

#include <iostream>

namespace ts::app {

void tiksrv_app::initialize() {
    logger_.info("Initializing application");
}

void tiksrv_app::run() {
    initialize();

    logger_.info("Running application");
}

} // namespace ts::app
