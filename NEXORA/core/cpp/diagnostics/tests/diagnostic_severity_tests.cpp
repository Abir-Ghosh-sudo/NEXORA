
#include <cstdlib>
#include <iostream>

#include <nexora/diagnostics/diagnostic_severity.hpp>

namespace {

int passed = 0;
int failed = 0;

void expect(bool condition, const char* message) {
    if (condition) {
        ++passed;
        std::cout << "[PASS] " << message << '\n';
    } else {
        ++failed;
        std::cerr << "[FAIL] " << message << '\n';
    }
}

void testSeverityNames() {
    using namespace nexora::diagnostics;

    expect(
        diagnosticSeverityName(DiagnosticSeverity::Trace) == "TRACE",
        "Trace severity name"
    );

    expect(
        diagnosticSeverityName(DiagnosticSeverity::Info) == "INFO",
        "Info severity name"
    );

    expect(
        diagnosticSeverityName(DiagnosticSeverity::Warning) == "WARNING",
        "Warning severity name"
    );

    expect(
        diagnosticSeverityName(DiagnosticSeverity::Error) == "ERROR",
        "Error severity name"
    );

    expect(
        diagnosticSeverityName(DiagnosticSeverity::Critical) == "CRITICAL",
        "Critical severity name"
    );
}

void testSeverityValidation() {
    using namespace nexora::diagnostics;

    expect(
        isValidDiagnosticSeverity(DiagnosticSeverity::Trace),
        "Trace is valid"
    );

    expect(
        isValidDiagnosticSeverity(DiagnosticSeverity::Critical),
        "Critical is valid"
    );

    expect(
        !isValidDiagnosticSeverity(
            static_cast<DiagnosticSeverity>(255)
        ),
        "Invalid severity is rejected"
    );
}

} // namespace

int main() {
    testSeverityNames();
    testSeverityValidation();

    std::cout << "\n================================\n";
    std::cout << "Diagnostic Severity Test Results\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "================================\n";

    return failed == 0
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}
