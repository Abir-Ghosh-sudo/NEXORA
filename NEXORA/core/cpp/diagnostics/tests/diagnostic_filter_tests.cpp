
#include <cstdlib>
#include <iostream>
#include <vector>

#include "nexora/diagnostics/diagnostic_filter.hpp"

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
        nexora::diagnostics::DiagnosticSeverity severity,
        const char* message
    )
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = severity;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = message;

        return event;
    }

    void testSeverityMatching()
    {
        using namespace nexora::diagnostics;

        DiagnosticFilter filter(DiagnosticSeverity::Warning);

        expect(
            !filter.matches(createEvent(DiagnosticSeverity::Trace, "Trace")),
            "Trace event is filtered out"
        );

        expect(
            !filter.matches(createEvent(DiagnosticSeverity::Info, "Info")),
            "Info event is filtered out"
        );

        expect(
            filter.matches(createEvent(DiagnosticSeverity::Warning, "Warning")),
            "Warning event is accepted"
        );

        expect(
            filter.matches(createEvent(DiagnosticSeverity::Error, "Error")),
            "Error event is accepted"
        );

        expect(
            filter.matches(createEvent(DiagnosticSeverity::Critical, "Critical")),
            "Critical event is accepted"
        );
    }

    void testFilterCollection()
    {
        using namespace nexora::diagnostics;

        DiagnosticFilter filter(DiagnosticSeverity::Error);

        const std::vector<DiagnosticEvent> events{
            createEvent(DiagnosticSeverity::Info, "Information"),
            createEvent(DiagnosticSeverity::Warning, "Warning"),
            createEvent(DiagnosticSeverity::Error, "Error"),
            createEvent(DiagnosticSeverity::Critical, "Critical")
        };

        const auto filtered = filter.filter(events);

        expect(filtered.size() == 2, "Filter returns two matching events");

        if (filtered.size() == 2)
        {
            expect(
                filtered[0].severity == DiagnosticSeverity::Error,
                "Filtered results preserve error event"
            );

            expect(
                filtered[1].severity == DiagnosticSeverity::Critical,
                "Filtered results preserve critical event"
            );
        }
    }

    void testTraceMinimumSeverity()
    {
        using namespace nexora::diagnostics;

        DiagnosticFilter filter(DiagnosticSeverity::Trace);

        expect(
            filter.matches(createEvent(DiagnosticSeverity::Trace, "Trace")),
            "Trace minimum severity accepts trace event"
        );

        expect(
            filter.matches(createEvent(DiagnosticSeverity::Critical, "Critical")),
            "Trace minimum severity accepts critical event"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Filter Tests ===\n\n";

    testSeverityMatching();
    testFilterCollection();
    testTraceMinimumSeverity();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic filter tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic filter tests failed!\n";
    return EXIT_FAILURE;
}
