
#include <cstdlib>
#include <iostream>

#include "nexora/diagnostics/diagnostic_location.hpp"

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

    void testValidLocation()
    {
        const nexora::diagnostics::DiagnosticLocation location{
            "runtime.cpp",
            "initializeRuntime",
            42,
            10
        };

        expect(location.isValid(), "Valid diagnostic location");
        expect(location.file == "runtime.cpp", "File name stored correctly");
        expect(location.function == "initializeRuntime", "Function name stored correctly");
        expect(location.line == 42, "Line number stored correctly");
        expect(location.column == 10, "Column number stored correctly");
    }

    void testEmptyLocation()
    {
        const nexora::diagnostics::DiagnosticLocation location{};

        expect(!location.isValid(), "Empty location is invalid");
    }

    void testPartialLocation()
    {
        const nexora::diagnostics::DiagnosticLocation location{
            "main.cpp",
            "",
            15,
            0
        };

        expect(location.isValid(), "Location with file and line is valid");
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Location Tests ===\n\n";

    testValidLocation();
    testEmptyLocation();
    testPartialLocation();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic location tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic location tests failed!\n";
    return EXIT_FAILURE;
}
