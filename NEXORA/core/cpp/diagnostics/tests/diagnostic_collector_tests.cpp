
#include <cstdlib>
#include <iostream>
#include <string>

#include "nexora/diagnostics/diagnostic_collector.hpp"

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

    void testCollectEvents()
    {
        nexora::diagnostics::DiagnosticCollector collector;

        collector.collect(createEvent("First error"));
        collector.collect(createEvent("Second error"));

        expect(collector.size() == 2, "Collector stores two events");
        expect(!collector.empty(), "Collector is not empty");
    }

    void testSnapshot()
    {
        nexora::diagnostics::DiagnosticCollector collector;

        collector.collect(createEvent("Runtime error"));
        collector.collect(createEvent("Memory error"));

        const auto snapshot = collector.snapshot();

        expect(snapshot.size() == 2, "Snapshot contains all collected events");

        if (snapshot.size() == 2)
        {
            expect(
                snapshot[0].message == "Runtime error",
                "Snapshot preserves first event"
            );

            expect(
                snapshot[1].message == "Memory error",
                "Snapshot preserves second event"
            );
        }
    }

    void testSnapshotIndependence()
    {
        nexora::diagnostics::DiagnosticCollector collector;

        collector.collect(createEvent("Original event"));

        auto snapshot = collector.snapshot();

        collector.collect(createEvent("New event"));

        expect(
            snapshot.size() == 1,
            "Existing snapshot remains unchanged after collection"
        );

        expect(
            collector.size() == 2,
            "Collector stores newly collected event"
        );
    }

    void testClear()
    {
        nexora::diagnostics::DiagnosticCollector collector;

        collector.collect(createEvent("Temporary event"));
        collector.clear();

        expect(collector.empty(), "Clear removes all events");
        expect(collector.size() == 0, "Clear resets collector size");
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Collector Tests ===\n\n";

    testCollectEvents();
    testSnapshot();
    testSnapshotIndependence();
    testClear();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic collector tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic collector tests failed!\n";
    return EXIT_FAILURE;
}
