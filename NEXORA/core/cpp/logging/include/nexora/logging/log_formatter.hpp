
#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <nexora/logging/log_record.hpp>

namespace nexora::logging {

class LogFormatter final {
public:
    [[nodiscard]]
    static std::string format(const LogRecord& record) {
        using namespace std::chrono;

        const auto time =
            LogRecord::Clock::to_time_t(record.timestamp);

        const auto millisecond_part =
            duration_cast<std::chrono::milliseconds>(
                record.timestamp.time_since_epoch()
            ).count() % 1000;

        std::tm time_info{};

#if defined(_WIN32)
        localtime_s(&time_info, &time);
#else
        localtime_r(&time, &time_info);
#endif

        std::ostringstream output;

        output << '['
               << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S")
               << '.'
               << std::setfill('0')
               << std::setw(3)
               << millisecond_part
               << "] ["
               << logLevelName(record.level)
               << "] [Thread: "
               << record.thread_id
               << "] "
               << record.message;

        return output.str();
    }
};

} // namespace nexora::logging
