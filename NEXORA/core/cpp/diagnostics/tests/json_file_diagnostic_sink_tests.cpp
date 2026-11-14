
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "nexora/diagnostics/json_file_diagnostic_sink.hpp"

namespace
{
    int failures = 0;

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

    std::string readFile(const std::string& path)
    {
        std::ifstream file(path);

        if (!file.is_open())
        {
            return {};
        }

        return std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );
    }

    void testFileCreationAndWrite()
    {
        const std::string path = "nexora_json_diagnostic_test.jsonl";
        std::remove(path.c_str());

        {
            nexora::diagnostics::JsonFileDiagnosticSink sink(path);

            expect(sink.isOpen(), "JSON diagnostic file opens");

            sink.write(createEvent("First JSON event"));
            sink.flush();
        }

        const std::string content = readFile(path);

        expect(!content.empty(), "JSON diagnostic file contains data");
        expect(
            content.find("First JSON event") != std::string::npos,
            "JSON diagnostic file contains written event"
        );

        std::remove(path.c_str());
    }

    void testAppendMode()
    {
        const std::string path = "nexora_json_diagnostic_append_test.jsonl";
        std::remove(path.c_str());

        {
            nexora::diagnostics::JsonFileDiagnosticSink sink(path);
            sink.write(createEvent("First event"));
            sink.flush();
        }

        {
            nexora::diagnostics::JsonFileDiagnosticSink sink(path);
            sink.write(createEvent("Second event"));
            sink.flush();
        }

        const std::string content = readFile(path);

        expect(
            content.find("First event") != std::string::npos,
            "Append mode preserves first event"
        );

        expect(
            content.find("Second event") != std::string::npos,
            "Append mode writes second event"
        );

        std::remove(path.c_str());
    }

    void testMultipleEvents()
    {
        const std::string path = "nexora_json_diagnostic_multiple_test.jsonl";
        std::remove(path.c_str());

        {
            nexora::diagnostics::JsonFileDiagnosticSink sink(path);

            sink.write(createEvent("Event A"));
            sink.write(createEvent("Event B"));
            sink.write(createEvent("Event C"));
            sink.flush();
        }

        const std::string content = readFile(path);

        expect(
            content.find("Event A") != std::string::npos &&
            content.find("Event B") != std::string::npos &&
            content.find("Event C") != std::string::npos,
            "JSON sink writes multiple events"
        );

        const auto first = content.find("Event A");
        const auto second = content.find("Event B");
        const auto third = content.find("Event C");

        expect(
            first < second && second < third,
            "JSON events retain write order"
        );

        std::remove(path.c_str());
    }
}

int main()
{
    std::cout << "\n=== NEXORA JSON File Diagnostic Sink Tests ===\n\n";

    testFileCreationAndWrite();
    testAppendMode();
    testMultipleEvents();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All JSON file diagnostic sink tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some JSON file diagnostic sink tests failed!\n";
    return EXIT_FAILURE;
}
