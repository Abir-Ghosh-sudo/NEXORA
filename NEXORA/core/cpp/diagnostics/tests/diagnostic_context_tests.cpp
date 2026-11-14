
#include <cstdlib>
#include <iostream>
#include <string>

#include "nexora/diagnostics/diagnostic_context.hpp"

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

    void testSetAndGet()
    {
        nexora::diagnostics::DiagnosticContext context;

        context.set("module", "runtime");
        context.set("operation", "initialize");

        const auto* module = context.get("module");
        const auto* operation = context.get("operation");

        expect(context.contains("module"), "Context contains inserted key");

        expect(
            module != nullptr && *module == "runtime",
            "Context retrieves stored value"
        );

        expect(
            operation != nullptr && *operation == "initialize",
            "Context retrieves second value"
        );
    }

    void testUpdateExistingKey()
    {
        nexora::diagnostics::DiagnosticContext context;

        context.set("module", "runtime");
        context.set("module", "logging");

        const auto* module = context.get("module");

        expect(
            module != nullptr && *module == "logging",
            "Existing context value can be updated"
        );

        expect(context.size() == 1, "Updating key does not increase size");
    }

    void testMissingKey()
    {
        const nexora::diagnostics::DiagnosticContext context;

        expect(!context.contains("missing"), "Missing key is not contained");

        expect(
            context.get("missing") == nullptr,
            "Missing key returns nullptr"
        );
    }

    void testClear()
    {
        nexora::diagnostics::DiagnosticContext context;

        context.set("module", "runtime");
        context.set("status", "failed");

        context.clear();

        expect(context.empty(), "Clear makes context empty");
        expect(context.size() == 0, "Clear resets context size");
    }

    void testMetadata()
    {
        nexora::diagnostics::DiagnosticContext context;

        context.set("service", "nexora");
        context.set("version", "0.1.0");

        const auto& metadata = context.metadata();

        expect(metadata.size() == 2, "Metadata exposes stored entries");

        expect(
            metadata.at("service") == "nexora",
            "Metadata contains expected service value"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Context Tests ===\n\n";

    testSetAndGet();
    testUpdateExistingKey();
    testMissingKey();
    testClear();
    testMetadata();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic context tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic context tests failed!\n";
    return EXIT_FAILURE;
}
