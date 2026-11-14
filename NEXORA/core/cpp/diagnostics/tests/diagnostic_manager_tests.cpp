
#include <cstdlib>
#include <iostream>
#include <string>

#include "nexora/diagnostics/diagnostic_manager.hpp"

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
        const std::string& message
    )
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = severity;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = message;

        return event;
    }

    void testRecordEvents()
    {
        using namespace nexora::diagnostics;

        DiagnosticManager manager;

        manager.record(createEvent(DiagnosticSeverity::Info, "System started"));
        manager.record(createEvent(DiagnosticSeverity::Error, "Runtime failed"));

        expect(manager.eventCount() == 2, "Manager records two events");
        expect(manager.totalCount() == 2, "Manager total count is correct");
        expect(manager.count(DiagnosticSeverity::Info) == 1, "Info count is correct");
        expect(manager.count(DiagnosticSeverity::Error) == 1, "Error count is correct");
    }

    void testRetrieveEvents()
    {
        using namespace nexora::diagnostics;

        DiagnosticManager manager;

        manager.record(createEvent(DiagnosticSeverity::Info, "First event"));
        manager.record(createEvent(DiagnosticSeverity::Warning, "Second event"));

        const auto events = manager.events();

        expect(events.size() == 2, "Manager retrieves all events");

        if (events.size() == 2)
        {
            expect(
                events[0].message == "First event",
                "First event is preserved"
            );

            expect(
                events[1].message == "Second event",
                "Second event is preserved"
            );
        }
    }

    void testSeverityFiltering()
    {
        using namespace nexora::diagnostics;

        DiagnosticManager manager;

        manager.record(createEvent(DiagnosticSeverity::Info, "Information"));
        manager.record(createEvent(DiagnosticSeverity::Warning, "Warning"));
        manager.record(createEvent(DiagnosticSeverity::Error, "Error"));

        manager.setMinimumSeverity(DiagnosticSeverity::Warning);

        const auto filtered = manager.filteredEvents();

        expect(
            filtered.size() == 2,
            "Manager filters events by minimum severity"
        );

        if (filtered.size() == 2)
        {
            expect(
                filtered[0].severity == DiagnosticSeverity::Warning,
                "Filtered events contain warning"
            );

            expect(
                filtered[1].severity == DiagnosticSeverity::Error,
                "Filtered events contain error"
            );
        }
    }

    void testClearManager()
    {
        using namespace nexora::diagnostics;

        DiagnosticManager manager;

        manager.record(createEvent(DiagnosticSeverity::Error, "Temporary error"));

        manager.clear();

        expect(manager.eventCount() == 0, "Clear removes stored events");
        expect(manager.totalCount() == 0, "Clear resets total statistics");
        expect(manager.events().empty(), "Clear returns empty event collection");
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Manager Tests ===\n\n";

    testRecordEvents();
    testRetrieveEvents();
    testSeverityFiltering();
    testClearManager();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic manager tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic manager tests failed!\n";
    return EXIT_FAILURE;
}
