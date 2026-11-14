
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "nexora/diagnostics/console_diagnostic_sink.hpp"

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

    void testConsoleOutput()
    {
        std::ostringstream output;

        nexora::diagnostics::ConsoleDiagnosticSink sink(output);

        sink.write(createEvent("Console test message"));

        const std::string result = output.str();

        expect(
            !result.empty(),
            "Console sink produces output"
        );

        expect(
            result.find("Console test message") != std::string::npos,
            "Console output contains event message"
        );

        expect(
            result.find("ERROR") != std::string::npos,
            "Console output contains severity"
        );
    }

    void testMultipleEvents()
    {
        std::ostringstream output;

        nexora::diagnostics::ConsoleDiagnosticSink sink(output);

        sink.write(createEvent("First message"));
        sink.write(createEvent("Second message"));

        const std::string result = output.str();

        const auto first = result.find("First message");
        const auto second = result.find("Second message");

        expect(
            first != std::string::npos && second != std::string::npos,
            "Console sink writes multiple events"
        );

        expect(
            first < second,
            "Console sink preserves event order"
        );
    }

    void testFlush()
    {
        std::ostringstream output;

        nexora::diagnostics::ConsoleDiagnosticSink sink(output);

        sink.write(createEvent("Flush test"));
        sink.flush();

        expect(
            output.str().find("Flush test") != std::string::npos,
            "Console sink flush preserves written output"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Console Diagnostic Sink Tests ===\n\n";

    testConsoleOutput();
    testMultipleEvents();
    testFlush();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All console diagnostic sink tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some console diagnostic sink tests failed!\n";
    return EXIT_FAILURE;
}
