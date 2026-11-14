
#include <cstdlib>
#include <iostream>

#include "nexora/diagnostics/diagnostic_statistics.hpp"

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
        nexora::diagnostics::DiagnosticSeverity severity
    )
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = severity;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = "Statistics test event";

        return event;
    }

    void testInitialStatistics()
    {
        using namespace nexora::diagnostics;

        DiagnosticStatistics statistics;

        expect(statistics.totalCount() == 0, "Initial total count is zero");

        expect(
            statistics.count(DiagnosticSeverity::Trace) == 0,
            "Initial trace count is zero"
        );

        expect(
            statistics.count(DiagnosticSeverity::Error) == 0,
            "Initial error count is zero"
        );
    }

    void testSeverityCounting()
    {
        using namespace nexora::diagnostics;

        DiagnosticStatistics statistics;

        statistics.record(createEvent(DiagnosticSeverity::Info));
        statistics.record(createEvent(DiagnosticSeverity::Info));
        statistics.record(createEvent(DiagnosticSeverity::Warning));
        statistics.record(createEvent(DiagnosticSeverity::Error));
        statistics.record(createEvent(DiagnosticSeverity::Critical));

        expect(
            statistics.count(DiagnosticSeverity::Info) == 2,
            "Info severity count is correct"
        );

        expect(
            statistics.count(DiagnosticSeverity::Warning) == 1,
            "Warning severity count is correct"
        );

        expect(
            statistics.count(DiagnosticSeverity::Error) == 1,
            "Error severity count is correct"
        );

        expect(
            statistics.count(DiagnosticSeverity::Critical) == 1,
            "Critical severity count is correct"
        );

        expect(
            statistics.totalCount() == 5,
            "Total diagnostic count is correct"
        );
    }

    void testResetStatistics()
    {
        using namespace nexora::diagnostics;

        DiagnosticStatistics statistics;

        statistics.record(createEvent(DiagnosticSeverity::Error));
        statistics.record(createEvent(DiagnosticSeverity::Critical));

        statistics.reset();

        expect(
            statistics.totalCount() == 0,
            "Reset clears total count"
        );

        expect(
            statistics.count(DiagnosticSeverity::Error) == 0,
            "Reset clears error count"
        );

        expect(
            statistics.count(DiagnosticSeverity::Critical) == 0,
            "Reset clears critical count"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Statistics Tests ===\n\n";

    testInitialStatistics();
    testSeverityCounting();
    testResetStatistics();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic statistics tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic statistics tests failed!\n";
    return EXIT_FAILURE;
}
