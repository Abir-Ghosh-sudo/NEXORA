
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include <nexora/logging/log_record.hpp>

namespace {

int passed = 0;
int failed = 0;

void expect(bool condition, const char* message) {
    if (condition) {
        ++passed;
        std::cout << "[PASS] " << message << '\n';
    } else {
        ++failed;
        std::cerr << "[FAIL] " << message << '\n';
    }
}

void testDefaultLogRecord() {
    const nexora::logging::LogRecord record{};

    expect(
        record.level == nexora::logging::LogLevel::Info,
        "Default log level should be INFO"
    );

    expect(
        record.message.empty(),
        "Default message should be empty"
    );

    expect(
        record.timestamp == nexora::logging::LogRecord::Timestamp{},
        "Default timestamp should be initialized"
    );

    expect(
        record.thread_id == std::thread::id{},
        "Default thread ID should be initialized"
    );
}

void testInitializedLogRecord() {
    using namespace nexora::logging;

    const auto timestamp = LogRecord::Clock::now();
    const auto thread_id = std::this_thread::get_id();

    const LogRecord record{
        timestamp,
        LogLevel::Warning,
        "Test warning message",
        thread_id
    };

    expect(
        record.timestamp == timestamp,
        "LogRecord should preserve its timestamp"
    );

    expect(
        record.level == LogLevel::Warning,
        "LogRecord should preserve its log level"
    );

    expect(
        record.message == "Test warning message",
        "LogRecord should preserve its message"
    );

    expect(
        record.thread_id == thread_id,
        "LogRecord should preserve its thread ID"
    );
}

} // namespace

int main() {
    testDefaultLogRecord();
    testInitializedLogRecord();

    std::cout << "\n================================\n";
    std::cout << "LogRecord Tests Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "================================\n";

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
