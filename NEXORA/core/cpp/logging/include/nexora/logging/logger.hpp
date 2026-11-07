
#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <nexora/logging/log_formatter.hpp>
#include <nexora/logging/log_record.hpp>
#include <nexora/logging/log_sink.hpp>

namespace nexora::logging {

class Logger final {
public:
    explicit Logger(LogLevel minimum_level = LogLevel::Info) noexcept
        : minimum_level_(minimum_level) {}

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void addSink(std::shared_ptr<LogSink> sink) {
        if (!sink) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        sinks_.push_back(std::move(sink));
    }

    void setMinimumLevel(LogLevel level) noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        minimum_level_ = level;
    }

    [[nodiscard]]
    LogLevel minimumLevel() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return minimum_level_;
    }

    void log(LogLevel level, std::string message) {
        if (level == LogLevel::Off) {
            return;
        }

        std::vector<std::shared_ptr<LogSink>> sinks;

        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (level < minimum_level_ ||
                minimum_level_ == LogLevel::Off) {
                return;
            }

            sinks = sinks_;
        }

        if (sinks.empty()) {
            return;
        }

        LogRecord record{
            LogRecord::Clock::now(),
            level,
            std::move(message),
            std::this_thread::get_id()
        };

        const std::string formatted =
            LogFormatter::format(record);

        for (const auto& sink : sinks) {
            sink->write(formatted);
        }
    }

    void trace(std::string message) {
        log(LogLevel::Trace, std::move(message));
    }

    void debug(std::string message) {
        log(LogLevel::Debug, std::move(message));
    }

    void info(std::string message) {
        log(LogLevel::Info, std::move(message));
    }

    void warning(std::string message) {
        log(LogLevel::Warning, std::move(message));
    }

    void error(std::string message) {
        log(LogLevel::Error, std::move(message));
    }

    void critical(std::string message) {
        log(LogLevel::Critical, std::move(message));
    }

private:
    mutable std::mutex mutex_;
    LogLevel minimum_level_;
    std::vector<std::shared_ptr<LogSink>> sinks_;
};

} // namespace nexora::logging
