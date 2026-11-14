
#include <cstdlib>
#include <iostream>
#include <string>

#include "nexora/diagnostics/diagnostic_macros.hpp"

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

    void testDefaultLocationMacro()
    {
        const auto location = NEXORA_DIAGNOSTIC_LOCATION;

        expect(
            !location.file.empty(),
            "Default macro captures source file"
        );

        expect(
            !location.function.empty(),
            "Default macro captures function name"
        );

        expect(
            location.line > 0,
            "Default macro captures line number"
        );
    }

    void testExplicitColumnMacro()
    {
        const auto location = NEXORA_DIAGNOSTIC_LOCATION_AT(25);

        expect(
            location.column == 25,
            "Explicit macro captures column number"
        );

        expect(
            !location.file.empty(),
            "Explicit macro captures source file"
        );

        expect(
            !location.function.empty(),
            "Explicit macro captures function name"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Macros Tests ===\n\n";

    testDefaultLocationMacro();
    testExplicitColumnMacro();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic macros tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic macros tests failed!\n";
    return EXIT_FAILURE;
}
