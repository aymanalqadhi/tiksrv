#ifndef TIKSRV_LOG_LOGGER_HPP
#define TIKSRV_LOG_LOGGER_HPP

#include "log/core.hpp"

#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <string>

namespace ts::log {

class logger {
  public:
    logger(const std::string &name) : logger_ {name, main_sink()} {
        logger_.set_level(
            static_cast<spdlog::level::level_enum>(global_level()));
    }

    template <typename... TArg>
    void trace(const char *fmt, TArg... args) {
        logger_.trace(fmt, args...);
    }

    template <typename... TArg>
    void debug(const char *fmt, TArg... args) {
        logger_.debug(fmt, args...);
    }

    template <typename... TArg>
    void info(const char *fmt, TArg... args) {
        logger_.info(fmt, args...);
    }

    template <typename... TArg>
    void warn(const char *fmt, TArg... args) {
        logger_.warn(fmt, args...);
    }

    template <typename... TArg>
    void error(const char *fmt, TArg... args) {
        logger_.error(fmt, args...);
    }

    template <typename... TArg>
    void fatal(const char *fmt, TArg... args) {
        logger_.critical(fmt, args...);
    }

  private:
    spdlog::sinks::stdout_color_sink_st sink_;
    spdlog::logger                      logger_;
};

} // namespace ts::log

#endif
