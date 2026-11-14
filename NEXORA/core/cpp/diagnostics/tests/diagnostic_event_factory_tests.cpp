
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "nexora/diagnostics/diagnostic_event_factory.hpp"

namespace
{
    using namespace nexora::diagnostics;

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

    void testCreatesEvent()
    {
        const auto event = DiagnosticEventFactory::create(
            DiagnosticSeverity::Error,
            DiagnosticCode::GeneralFailure,
            "Factory test event"
        );

        expect(
            event.message == "Factory test event",
            "Factory creates event with correct message"
        );

        expect(
            event.severity == DiagnosticSeverity::Error,
            "Factory sets correct severity"
        );

        expect(
            event.code == DiagnosticCode::GeneralFailure,
            "Factory sets correct diagnostic code"
        );
    }

    void testDefaultLocation()
    {
        const auto event = DiagnosticEventFactory::create(
            DiagnosticSeverity::Warning,
            DiagnosticCode::InvalidArgument,
            "Default location test"
        );

        expect(
            event.location.file.empty() &&
            event.location.function.empty(),
            "Factory supports default location"
        );
    }

    void testCustomLocation()
    {
        const DiagnosticLocation location{
            "runtime.cpp",
            "initializeRuntime",
            42,
            8
        };

        const auto event = DiagnosticEventFactory::create(
            DiagnosticSeverity::Critical,
            DiagnosticCode::RuntimeInitializationFailed,
            "Runtime initialization failed",
            location
        );

        expect(
            event.location.file == "runtime.cpp",
            "Factory preserves source file"
        );

        expect(
            event.location.function == "initializeRuntime",
            "Factory preserves function name"
        );

        expect(
            event.location.line == 42 &&
            event.location.column == 8,
            "Factory preserves source position"
        );
    }

    void testTimestamp()
    {
        const auto before = DiagnosticEvent::Clock::now();

        const auto event = DiagnosticEventFactory::create(
            DiagnosticSeverity::Info,
            DiagnosticCode::Unknown,
            "Timestamp test"
        );

        const auto after = DiagnosticEvent::Clock::now();

        expect(
            event.timestamp >= before &&
            event.timestamp <= after,
            "Factory assigns current timestamp"
        );
    }

    void testThreadId()
    {
        const auto event = DiagnosticEventFactory::create(
            DiagnosticSeverity::Info,
            DiagnosticCode::Unknown,
            "Thread ID test"
        );

        expect(
            event.thread_id == std::this_thread::get_id(),
            "Factory assigns current thread ID"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Event Factory Tests ===\n\n";

    testCreatesEvent();
    testDefaultLocation();
    testCustomLocation();
    testTimestamp();
    testThreadId();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic event factory tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic event factory tests failed!\n";
    return EXIT_FAILURE;
}
