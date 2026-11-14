
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "nexora/diagnostics/file_diagnostic_sink.hpp"

namespace
{
    int failures = 0;

    constexpr const char* TEST_FILE =
        "nexora_diagnostic_sink_test.log";

    void expect(bool condition, const char* testName)
    {
        if (condition)
        {
            std::cout << "[PASS] " << testName << '\n';
        }
        else
        {
            std::cerr << "[FAIL] " << testName << '\n';
            ++failures;
        }
    }

    nexora::diagnostics::DiagnosticEvent createEvent(
        const std::string& message
    )
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = DiagnosticSeverity::Error;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = message;

        return event;
    }

    std::string readFile()
    {
        std::ifstream input(TEST_FILE);

        if (!input.is_open())
        {
            return {};
        }

        return std::string(
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()
        );
    }

    void cleanup()
    {
        std::remove(TEST_FILE);
    }

    void testFileCreationAndWrite()
    {
        cleanup();

        {
            nexora::diagnostics::FileDiagnosticSink sink(TEST_FILE);

            expect(sink.isOpen(), "File sink opens output file");

            sink.write(createEvent("First file event"));
            sink.flush();
        }

        const std::string content = readFile();

        expect(
            content.find("First file event") != std::string::npos,
            "File contains written diagnostic event"
        );

        cleanup();
    }

    void testAppendMode()
    {
        cleanup();

        {
            nexora::diagnostics::FileDiagnosticSink sink(TEST_FILE);

            sink.write(createEvent("First event"));
            sink.flush();
        }

        {
            nexora::diagnostics::FileDiagnosticSink sink(TEST_FILE);

            sink.write(createEvent("Second event"));
            sink.flush();
        }

        const std::string content = readFile();

        expect(
            content.find("First event") != std::string::npos,
            "Append mode preserves existing content"
        );

        expect(
            content.find("Second event") != std::string::npos,
            "Append mode writes new content"
        );

        cleanup();
    }

    void testMultipleEvents()
    {
        cleanup();

        {
            nexora::diagnostics::FileDiagnosticSink sink(TEST_FILE);

            sink.write(createEvent("Event A"));
            sink.write(createEvent("Event B"));
            sink.write(createEvent("Event C"));

            sink.flush();
        }

        const std::string content = readFile();

        const auto first = content.find("Event A");
        const auto second = content.find("Event B");
        const auto third = content.find("Event C");

        expect(
            first != std::string::npos &&
            second != std::string::npos &&
            third != std::string::npos,
            "File contains all written events"
        );

        expect(
            first < second && second < third,
            "File preserves event order"
        );

        cleanup();
    }
}

int main()
{
    std::cout << "\n=== NEXORA File Diagnostic Sink Tests ===\n\n";

    testFileCreationAndWrite();
    testAppendMode();
    testMultipleEvents();

    cleanup();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All file diagnostic sink tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some file diagnostic sink tests failed!\n";
    return EXIT_FAILURE;
}
