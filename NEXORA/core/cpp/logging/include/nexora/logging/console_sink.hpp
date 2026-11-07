
#pragma once

#include <iostream>
#include <mutex>
#include <string_view>

namespace nexora::logging {

class ConsoleSink final {
public:
    ConsoleSink() = default;

    ConsoleSink(const ConsoleSink&) = delete;
    ConsoleSink& operator=(const ConsoleSink&) = delete;

    void write(std::string_view message) {
        std::lock_guard<std::mutex> lock(mutex_);

        std::clog << message << '\n';
        std::clog.flush();
    }

private:
    std::mutex mutex_;
};

} // namespace nexora::logging
