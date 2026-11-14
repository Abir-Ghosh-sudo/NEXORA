
#include <cstdlib>
#include <iostream>
#include <string>

#include "nexora/diagnostics/diagnostic_reporter.hpp"

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

    class TestDiagnosticReporter final
        : public nexora::diagnostics::DiagnosticReporter
    {
    public:
        std::string report(
            const nexora::diagnostics::DiagnosticEvent& event
        ) const override
        {
            return "REPORT: " + event.message;
        }
    };

    nexora::diagnostics::DiagnosticEvent createEvent()
    {
        using namespace nexora::diagnostics;

        DiagnosticEvent event{};
        event.severity = DiagnosticSeverity::Error;
        event.code = DiagnosticCode::GeneralFailure;
        event.message = "Runtime initialization failed";

        return event;
    }

    void testReporterImplementation()
    {
        TestDiagnosticReporter reporter;
        const auto event = createEvent();

        const std::string output = reporter.report(event);

        expect(
            output == "REPORT: Runtime initialization failed",
            "Reporter generates expected report"
        );
    }

    void testPolymorphicReporter()
    {
        TestDiagnosticReporter reporter;

        const nexora::diagnostics::DiagnosticReporter& base = reporter;
        const auto event = createEvent();

        const std::string output = base.report(event);

        expect(
            output.find("Runtime initialization failed") != std::string::npos,
            "Reporter works through base interface"
        );
    }

    void testEmptyMessage()
    {
        TestDiagnosticReporter reporter;

        nexora::diagnostics::DiagnosticEvent event{};

        const std::string output = reporter.report(event);

        expect(
            output == "REPORT: ",
            "Reporter handles empty diagnostic message"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Reporter Tests ===\n\n";

    testReporterImplementation();
    testPolymorphicReporter();
    testEmptyMessage();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic reporter tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic reporter tests failed!\n";
    return EXIT_FAILURE;
}
