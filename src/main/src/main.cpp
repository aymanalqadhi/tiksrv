#include "app/app.hpp"
#include "config/config.hpp"
#include "log/core.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

#include <filesystem>
#include <iostream>
#include <vector>

using ts::app::tiksrv_app;
using ts::config::config;
using ts::config::config_key;

auto main(const int argc, char *argv[]) -> int {
    try {
        auto conf = config::from_argv({argv, argv + argc});

        if (conf.has(config_key::help)) {
            conf >> std::cout;
            return EXIT_SUCCESS;
        }

        if (conf.has(config_key::version)) {
            std::cout << config::constants::app_name << ": "
                      << config::constants::app_version << std::endl;
            return EXIT_SUCCESS;
        }

        ts::log::add_sink<spdlog::sinks::stderr_color_sink_st>(
            ts::log::log_level::trace);
        ts::log::global_level(ts::log::log_level::trace);

        tiksrv_app app {conf};
        app.run();
    } catch (const std::exception &ex) {
        std::clog << "[FATAL] " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
