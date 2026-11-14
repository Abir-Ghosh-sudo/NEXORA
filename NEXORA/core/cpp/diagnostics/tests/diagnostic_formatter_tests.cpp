
#include <cstdlib>
#include <iostream>
#include <string>

#include "nexora/diagnostics/diagnostic_formatter.hpp"

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

    nexora::diagnostics::DiagnosticEvent createEvent()
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = DiagnosticSeverity::Error;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = "Runtime initialization failed";
        event.location = DiagnosticLocation{
            "runtime.cpp",
            "initializeRuntime",
            42,
            8
        };

        return event;
    }

    void testFormattedOutput()
    {
        const auto event = createEvent();

        const std::string output =
            nexora::diagnostics::DiagnosticFormatter::format(event);

        expect(!output.empty(), "Formatter produces non-empty output");

        expect(
            output.find("Runtime initialization failed") != std::string::npos,
            "Formatted output contains diagnostic message"
        );

        expect(
            output.find("ERROR") != std::string::npos,
            "Formatted output contains severity"
        );

        expect(
            output.find("runtime.cpp") != std::string::npos,
            "Formatted output contains source file"
        );

        expect(
            output.find("initializeRuntime") != std::string::npos,
            "Formatted output contains function name"
        );
    }

    void testFormattedOutputWithoutLocation()
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = DiagnosticSeverity::Info;
        event.code = DiagnosticCode::Unknown;
        event.message = "System started";

        const std::string output = DiagnosticFormatter::format(event);

        expect(
            output.find("System started") != std::string::npos,
            "Formatter handles event without source location"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Formatter Tests ===\n\n";

    testFormattedOutput();
    testFormattedOutputWithoutLocation();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic formatter tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic formatter tests failed!\n";
    return EXIT_FAILURE;
}
