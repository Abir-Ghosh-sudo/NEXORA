
#include <cstdlib>
#include <iostream>

#include <nexora/logging/log_level.hpp>

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

void testLogLevelNames() {
    using namespace nexora::logging;

    expect(logLevelName(LogLevel::Trace) == "TRACE",
           "Trace level name is correct");

    expect(logLevelName(LogLevel::Debug) == "DEBUG",
           "Debug level name is correct");

    expect(logLevelName(LogLevel::Info) == "INFO",
           "Info level name is correct");

    expect(logLevelName(LogLevel::Warning) == "WARNING",
           "Warning level name is correct");

    expect(logLevelName(LogLevel::Error) == "ERROR",
           "Error level name is correct");

    expect(logLevelName(LogLevel::Critical) == "CRITICAL",
           "Critical level name is correct");

    expect(logLevelName(LogLevel::Off) == "OFF",
           "Off level name is correct");
}

void testValidLogLevels() {
    using namespace nexora::logging;

    expect(isValidLogLevel(LogLevel::Trace),
           "Trace is valid");

    expect(isValidLogLevel(LogLevel::Debug),
           "Debug is valid");

    expect(isValidLogLevel(LogLevel::Info),
           "Info is valid");

    expect(isValidLogLevel(LogLevel::Warning),
           "Warning is valid");

    expect(isValidLogLevel(LogLevel::Error),
           "Error is valid");

    expect(isValidLogLevel(LogLevel::Critical),
           "Critical is valid");

    expect(isValidLogLevel(LogLevel::Off),
           "Off is valid");
}

void testInvalidLogLevel() {
    using namespace nexora::logging;

    const auto invalid =
        static_cast<LogLevel>(255);

    expect(!isValidLogLevel(invalid),
           "Invalid log level is rejected");

    expect(logLevelName(invalid) == "UNKNOWN",
           "Unknown log level returns fallback name");
}

} // namespace

int main() {
    testLogLevelNames();
    testValidLogLevels();
    testInvalidLogLevel();

    std::cout << "\nLog Level Test Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
