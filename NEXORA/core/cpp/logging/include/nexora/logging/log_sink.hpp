
#pragma once

#include <string_view>

namespace nexora::logging {

class LogSink {
public:
    LogSink() = default;

    virtual ~LogSink() = default;

    LogSink(const LogSink&) = delete;
    LogSink& operator=(const LogSink&) = delete;

    LogSink(LogSink&&) = delete;
    LogSink& operator=(LogSink&&) = delete;

    virtual void write(std::string_view message) = 0;

};

} // namespace nexora::logging
