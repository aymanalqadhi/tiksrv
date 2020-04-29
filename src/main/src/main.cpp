#include "app/config.hpp"

#include <filesystem>
#include <iostream>

using ts::app::config;
using ts::app::config_key;

int main(int argc, char *argv[]) {
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
    } catch (const std::exception &ex) {
        std::clog << "[FATAL] " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
