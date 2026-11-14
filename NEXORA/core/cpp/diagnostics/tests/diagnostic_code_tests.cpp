
#include <cstdlib>
#include <iostream>

#include "nexora/diagnostics/diagnostic_code.hpp"

namespace
{
    int failures = 0;

    void expect(
        bool condition,
        const char* testName
    )
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

    void testGeneralDiagnosticCodes()
    {
        using nexora::diagnostics::DiagnosticCode;
        using nexora::diagnostics::diagnosticCodeName;

        expect(
            diagnosticCodeName(DiagnosticCode::Unknown) == "UNKNOWN",
            "Unknown diagnostic code"
        );

        expect(
            diagnosticCodeName(DiagnosticCode::GeneralFailure) == "GENERAL_FAILURE",
            "General failure code"
        );

        expect(
            diagnosticCodeName(DiagnosticCode::InvalidArgument) == "INVALID_ARGUMENT",
            "Invalid argument code"
        );

        expect(
            diagnosticCodeName(DiagnosticCode::InvalidState) == "INVALID_STATE",
            "Invalid state code"
        );

        expect(
            diagnosticCodeName(DiagnosticCode::OperationFailed) == "OPERATION_FAILED",
            "Operation failed code"
        );
    }

    void testRuntimeAndMemoryCodes()
    {
        using nexora::diagnostics::DiagnosticCode;
        using nexora::diagnostics::diagnosticCodeName;

        expect(
            diagnosticCodeName(DiagnosticCode::RuntimeInitializationFailed)
                == "RUNTIME_INITIALIZATION_FAILED",
            "Runtime initialization failed code"
        );

        expect(
            diagnosticCodeName(DiagnosticCode::RuntimeShutdownFailed)
                == "RUNTIME_SHUTDOWN_FAILED",
            "Runtime shutdown failed code"
        );

        expect(
            diagnosticCodeName(DiagnosticCode::RuntimeConfigurationFailed)
                == "RUNTIME_CONFIGURATION_FAILED",
            "Runtime configuration failed code"
        );

        expect(
            diagnosticCodeName(DiagnosticCode::MemoryAllocationFailed)
                == "MEMORY_ALLOCATION_FAILED",
            "Memory allocation failed code"
        );

        expect(
            diagnosticCodeName(DiagnosticCode::MemoryLimitExceeded)
                == "MEMORY_LIMIT_EXCEEDED",
            "Memory limit exceeded code"
        );
    }

    void testInvalidDiagnosticCode()
    {
        using nexora::diagnostics::DiagnosticCode;
        using nexora::diagnostics::diagnosticCodeName;

        const auto invalidCode =
            static_cast<DiagnosticCode>(0xFFFFFFFFu);

        expect(
            diagnosticCodeName(invalidCode)
                == "UNRECOGNIZED_DIAGNOSTIC_CODE",
            "Unrecognized diagnostic code fallback"
        );
    }
}

int main()
{
    std::cout << "\n=== NEXORA Diagnostic Code Tests ===\n\n";

    testGeneralDiagnosticCodes();
    testRuntimeAndMemoryCodes();
    testInvalidDiagnosticCode();

    std::cout << "\n-----------------------------\n";
    std::cout << "Total failures: " << failures << '\n';

    if (failures == 0)
    {
        std::cout << "All diagnostic code tests passed!\n";
        return EXIT_SUCCESS;
    }

    std::cerr << "Some diagnostic code tests failed!\n";
    return EXIT_FAILURE;
}
