
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <nexora/logging/logger.hpp>

namespace {

using namespace nexora::logging;

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

class MemorySink final : public LogSink {
public:
    void write(std::string_view message) override {
        messages.emplace_back(message);
    }

    std::vector<std::string> messages;
};

void testSingleLogMessage() {
    Logger logger(LogLevel::Info);

    auto sink = std::make_shared<MemorySink>();
    logger.addSink(sink);

    logger.info("NEXORA initialized");

    expect(
        sink->messages.size() == 1,
        "Logger should write a single message"
    );

    expect(
        sink->messages[0].find("NEXORA initialized") != std::string::npos,
        "Logger should preserve the original message"
    );
}

void testMinimumLogLevel() {
    Logger logger(LogLevel::Warning);

    auto sink = std::make_shared<MemorySink>();
    logger.addSink(sink);

    logger.debug("Debug message");
    logger.info("Info message");
    logger.warning("Warning message");
    logger.error("Error message");

    expect(
        sink->messages.size() == 2,
        "Logger should filter messages below minimum level"
    );
}

void testOffLogLevel() {
    Logger logger(LogLevel::Off);

    auto sink = std::make_shared<MemorySink>();
    logger.addSink(sink);

    logger.critical("Critical message");

    expect(
        sink->messages.empty(),
        "Logger should not write messages when minimum level is OFF"
    );
}

void testExplicitOffMessage() {
    Logger logger(LogLevel::Trace);

    auto sink = std::make_shared<MemorySink>();
    logger.addSink(sink);

    logger.log(LogLevel::Off, "Disabled message");

    expect(
        sink->messages.empty(),
        "Logger should ignore messages with OFF level"
    );
}

void testMultipleSinks() {
    Logger logger(LogLevel::Info);

    auto first_sink = std::make_shared<MemorySink>();
    auto second_sink = std::make_shared<MemorySink>();

    logger.addSink(first_sink);
    logger.addSink(second_sink);

    logger.info("Shared message");

    expect(
        first_sink->messages.size() == 1 &&
        second_sink->messages.size() == 1,
        "Logger should dispatch messages to multiple sinks"
    );
}

void testNullSink() {
    Logger logger(LogLevel::Info);

    logger.addSink(nullptr);
    logger.info("Message without sink");

    expect(
        logger.minimumLevel() == LogLevel::Info,
        "Logger should safely ignore a null sink"
    );
}

void testSetMinimumLevel() {
    Logger logger(LogLevel::Debug);

    logger.setMinimumLevel(LogLevel::Error);

    expect(
        logger.minimumLevel() == LogLevel::Error,
        "Logger should update its minimum log level"
    );

    auto sink = std::make_shared<MemorySink>();
    logger.addSink(sink);

    logger.warning("Filtered warning");
    logger.error("Allowed error");

    expect(
        sink->messages.size() == 1,
        "Updated minimum level should affect log filtering"
    );
}

void testLogConvenienceMethods() {
    Logger logger(LogLevel::Trace);

    auto sink = std::make_shared<MemorySink>();
    logger.addSink(sink);

    logger.trace("Trace");
    logger.debug("Debug");
    logger.info("Info");
    logger.warning("Warning");
    logger.error("Error");
    logger.critical("Critical");

    expect(
        sink->messages.size() == 6,
        "All logger convenience methods should work"
    );
}

} // namespace

int main() {
    testSingleLogMessage();
    testMinimumLogLevel();
    testOffLogLevel();
    testExplicitOffMessage();
    testMultipleSinks();
    testNullSink();
    testSetMinimumLevel();
    testLogConvenienceMethods();

    std::cout << "\n================================\n";
    std::cout << "Logger Tests Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "================================\n";

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
