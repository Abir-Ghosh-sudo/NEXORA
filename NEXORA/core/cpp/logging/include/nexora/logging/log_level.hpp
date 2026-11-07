
#pragma once

#include <string_view>

namespace nexora::logging {

enum class LogLevel : unsigned char {
    Trace = 0,
    Debug,
    Info,
    Warning,
    Error,
    Critical,
    Off
};

[[nodiscard]]
constexpr std::string_view logLevelName(LogLevel level) noexcept {
    switch (level) {
        case LogLevel::Trace:
            return "TRACE";

        case LogLevel::Debug:
            return "DEBUG";

        case LogLevel::Info:
            return "INFO";

        case LogLevel::Warning:
            return "WARNING";

        case LogLevel::Error:
            return "ERROR";

        case LogLevel::Critical:
            return "CRITICAL";

        case LogLevel::Off:
            return "OFF";
    }

    return "UNKNOWN";
}

[[nodiscard]]
constexpr bool isValidLogLevel(LogLevel level) noexcept {
    return level >= LogLevel::Trace &&
           level <= LogLevel::Off;
}

} // namespace nexora::logging
