
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "nexora/diagnostics/diagnostic_event.hpp"

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

    void testDefaultEvent()
    {
        const nexora::diagnostics::DiagnosticEvent event{};

        expect(
            !event.isValid(),
            "Default diagnostic event is invalid"
        );

        expect(
            event.message.empty(),
            "Default event has empty message"
        );
    }

    void testValidEvent()
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};

        event.severity = DiagnosticSeverity::Error;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = "Runtime initialization failed";
        event.location = DiagnosticLocation{
            "runtime.cpp",
            "initialize",
            100,
            5
        };
        event.timestamp = DiagnosticEvent::Clock::now();
        event.thread_id = std::this_thread::get_id();

        expect(event.isValid(), "Valid diagnostic event");
        expect(event.severity == DiagnosticSeverity::Error, "Event severity stored");
        expect(event.code == DiagnosticCode::GeneralFailure, "Event code stored");
        expect(
            event.message == "Runtime initialization failed",
            "Event message stored"
        );
        expect(
            event.location.file == "runtime.cpp",
            "Event location stored"
        );
        expect(
            event.thread_id == std::this_thread::get_id(),
            "Event thread ID stored"
        );
    }

    void testEmptyMessage()
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = DiagnosticSeverity::Warning;
        event.code = DiagnosticCode::GeneralFailure;
        event.message.clear();

        expect(
            !event.isValid(),
            "Event with empty message is invalid"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Event Tests ===\n\n";

    testDefaultEvent();
    testValidEvent();
    testEmptyMessage();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic event tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic event tests failed!\n";
    return EXIT_FAILURE;
}
