
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include <nexora/logging/log_formatter.hpp>

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

void testFormattedLogContainsLevel() {
    using namespace nexora::logging;

    const LogRecord record{
        LogRecord::Clock::now(),
        LogLevel::Error,
        "Database connection failed",
        std::this_thread::get_id()
    };

    const std::string formatted = LogFormatter::format(record);

    expect(
        formatted.find("[ERROR]") != std::string::npos,
        "Formatted log should contain the log level"
    );
}

void testFormattedLogContainsMessage() {
    using namespace nexora::logging;

    const LogRecord record{
        LogRecord::Clock::now(),
        LogLevel::Info,
        "NEXORA runtime initialized",
        std::this_thread::get_id()
    };

    const std::string formatted = LogFormatter::format(record);

    expect(
        formatted.find("NEXORA runtime initialized") != std::string::npos,
        "Formatted log should contain the original message"
    );
}

void testFormattedLogContainsThreadId() {
    using namespace nexora::logging;

    const auto thread_id = std::this_thread::get_id();

    const LogRecord record{
        LogRecord::Clock::now(),
        LogLevel::Debug,
        "Thread test",
        thread_id
    };

    const std::string formatted = LogFormatter::format(record);

    expect(
        formatted.find("[Thread: ") != std::string::npos,
        "Formatted log should contain thread information"
    );
}

void testFormattedLogStructure() {
    using namespace nexora::logging;

    const LogRecord record{
        LogRecord::Clock::now(),
        LogLevel::Warning,
        "Formatter structure test",
        std::this_thread::get_id()
    };

    const std::string formatted = LogFormatter::format(record);

    expect(
        !formatted.empty() && formatted.front() == '[',
        "Formatted log should start with a timestamp bracket"
    );

    expect(
        formatted.find("] [WARNING] [Thread: ") != std::string::npos,
        "Formatted log should follow the expected structure"
    );
}

} // namespace

int main() {
    testFormattedLogContainsLevel();
    testFormattedLogContainsMessage();
    testFormattedLogContainsThreadId();
    testFormattedLogStructure();

    std::cout << "\n================================\n";
    std::cout << "LogFormatter Tests Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "================================\n";

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
