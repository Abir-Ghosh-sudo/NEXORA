
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

#include "nexora/diagnostics/diagnostic_result.hpp"

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

    nexora::diagnostics::DiagnosticEvent createTestEvent()
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = DiagnosticSeverity::Error;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = "Test diagnostic failure";

        return event;
    }

    void testSuccessResult()
    {
        using nexora::diagnostics::DiagnosticResult;

        const auto result = DiagnosticResult::success();

        expect(result.succeeded(), "Success result reports succeeded");
        expect(!result.failed(), "Success result does not report failed");
        expect(!result.event().has_value(), "Success result has no event");
    }

    void testFailureResult()
    {
        using nexora::diagnostics::DiagnosticResult;

        const auto event = createTestEvent();
        const auto result = DiagnosticResult::failure(event);

        expect(result.failed(), "Failure result reports failed");
        expect(!result.succeeded(), "Failure result does not report succeeded");
        expect(result.event().has_value(), "Failure result contains event");

        if (result.event().has_value())
        {
            expect(
                result.event()->message == "Test diagnostic failure",
                "Failure event message preserved"
            );
        }
    }

    void testFailureResultWithMove()
    {
        using nexora::diagnostics::DiagnosticResult;

        auto event = createTestEvent();
        auto result = DiagnosticResult::failure(std::move(event));

        expect(
            result.failed() && result.event().has_value(),
            "Failure result accepts moved event"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Result Tests ===\n\n";

    testSuccessResult();
    testFailureResult();
    testFailureResultWithMove();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic result tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic result tests failed!\n";
    return EXIT_FAILURE;
}
