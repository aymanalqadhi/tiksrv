#include "log/core.hpp"

#include "spdlog/sinks/dist_sink.h"
#include "spdlog/sinks/sink.h"

#include <memory>
#include <vector>

namespace {

std::shared_ptr<spdlog::sinks::dist_sink_st> main_sink {nullptr};

}

namespace ts::log {

void add_sink(spdlog::sink_ptr sink) {
    if (!::main_sink) {
        ::main_sink = std::make_shared<spdlog::sinks::dist_sink_st>();
    }

    ::main_sink->add_sink(std::move(sink));
}

std::shared_ptr<spdlog::sinks::dist_sink_st> &main_sink() {
    return ::main_sink;
}

} // namespace ts::log
