
#pragma once

#include <chrono>
#include <string>
#include <thread>

#include <nexora/logging/log_level.hpp>

namespace nexora::logging {

struct LogRecord final {
    using Clock = std::chrono::system_clock;
    using Timestamp = Clock::time_point;

    Timestamp timestamp{};
    LogLevel level{LogLevel::Info};
    std::string message{};
    std::thread::id thread_id{};
};

} // namespace nexora::logging
