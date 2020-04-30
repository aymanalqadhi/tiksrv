#include "app/app.hpp"
#include "config/config.hpp"
#include "log/core.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

#include <filesystem>
#include <iostream>

using ts::app::tiksrv_app;
using ts::config::config;
using ts::config::config_key;

auto main(int argc, char *argv[]) -> int {
    try {
        auto conf = config::from_argv(argc, argv);

        if (conf.has(config_key::help)) {
            conf >> std::cout;
            return EXIT_SUCCESS;
        }

        if (conf.has(config_key::version)) {
            std::cout << config::constants::app_name << ": "
                      << config::constants::app_version << std::endl;
            return EXIT_SUCCESS;
        }

        if (auto config_path = conf[config_key::config_file].as<std::string>();
            std::filesystem::exists(config_path)) {
            conf.parse_config_file(config_path);
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
