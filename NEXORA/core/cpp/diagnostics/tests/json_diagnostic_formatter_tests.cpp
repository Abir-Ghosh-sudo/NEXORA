
#include <cstdlib>
#include <iostream>
#include <string>

#include "nexora/diagnostics/json_diagnostic_formatter.hpp"

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
        event.location = DiagnosticLocation{
            "runtime.cpp",
            "initializeRuntime",
            42,
            8
        };

        return event;
    }

    void testJsonOutput()
    {
        const auto event = createEvent("Runtime initialization failed");

        const std::string json =
            nexora::diagnostics::JsonDiagnosticFormatter::format(event);

        expect(!json.empty(), "JSON formatter produces output");

        expect(
            json.front() == '{' && json.back() == '}',
            "JSON output has object boundaries"
        );

        expect(
            json.find("\"message\"") != std::string::npos,
            "JSON output contains message field"
        );

        expect(
            json.find("Runtime initialization failed") != std::string::npos,
            "JSON output contains event message"
        );

        expect(
            json.find("\"severity\"") != std::string::npos,
            "JSON output contains severity field"
        );
    }

    void testLocationFields()
    {
        const auto event = createEvent("Location test");

        const std::string json =
            nexora::diagnostics::JsonDiagnosticFormatter::format(event);

        expect(
            json.find("runtime.cpp") != std::string::npos,
            "JSON output contains source file"
        );

        expect(
            json.find("initializeRuntime") != std::string::npos,
            "JSON output contains function name"
        );
    }

    void testSpecialCharacterEscaping()
    {
        const auto event = createEvent(
            "Error with \"quotes\" and \\ backslash"
        );

        const std::string json =
            nexora::diagnostics::JsonDiagnosticFormatter::format(event);

        expect(
            json.find("\\\"quotes\\\"") != std::string::npos,
            "JSON formatter escapes quotation marks"
        );

        expect(
            json.find("\\\\") != std::string::npos,
            "JSON formatter escapes backslashes"
        );
    }

    void testEmptyMessage()
    {
        const auto event = createEvent("");

        const std::string json =
            nexora::diagnostics::JsonDiagnosticFormatter::format(event);

        expect(
            json.find("\"message\"") != std::string::npos,
            "JSON formatter handles empty message"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA JSON Diagnostic Formatter Tests ===\n\n";

    testJsonOutput();
    testLocationFields();
    testSpecialCharacterEscaping();
    testEmptyMessage();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All JSON diagnostic formatter tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some JSON diagnostic formatter tests failed!\n";
    return EXIT_FAILURE;
}
