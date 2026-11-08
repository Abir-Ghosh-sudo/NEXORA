
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include <nexora/logging/console_sink.hpp>

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

void testSingleMessage() {
    using namespace nexora::logging;

    std::ostringstream captured;
    auto* original_buffer = std::clog.rdbuf(captured.rdbuf());

    {
        ConsoleSink sink;
        sink.write("NEXORA logging test");
    }

    std::clog.rdbuf(original_buffer);

    expect(
        captured.str().find("NEXORA logging test") != std::string::npos,
        "ConsoleSink should write a single message"
    );
}

void testMultipleMessages() {
    using namespace nexora::logging;

    std::ostringstream captured;
    auto* original_buffer = std::clog.rdbuf(captured.rdbuf());

    {
        ConsoleSink sink;
        sink.write("First message");
        sink.write("Second message");
    }

    std::clog.rdbuf(original_buffer);

    const std::string output = captured.str();

    expect(
        output.find("First message") != std::string::npos &&
        output.find("Second message") != std::string::npos,
        "ConsoleSink should write multiple messages"
    );
}

void testEmptyMessage() {
    using namespace nexora::logging;

    std::ostringstream captured;
    auto* original_buffer = std::clog.rdbuf(captured.rdbuf());

    {
        ConsoleSink sink;
        sink.write("");
    }

    std::clog.rdbuf(original_buffer);

    expect(
        captured.str() == "\n",
        "ConsoleSink should handle an empty message"
    );
}

} // namespace

int main() {
    testSingleMessage();
    testMultipleMessages();
    testEmptyMessage();

    std::cout << "\n================================\n";
    std::cout << "ConsoleSink Tests Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "================================\n";

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
