
#pragma once

#include <cstdint>
#include <string_view>

namespace nexora::diagnostics {

enum class DiagnosticCode : std::uint32_t {

    // General diagnostics
    Unknown = 0x00000000,
    GeneralFailure = 0x00000001,
    InvalidArgument = 0x00000002,
    InvalidState = 0x00000003,
    OperationFailed = 0x00000004,

    // Runtime diagnostics
    RuntimeInitializationFailed = 0x00010001,
    RuntimeShutdownFailed = 0x00010002,
    RuntimeConfigurationFailed = 0x00010003,

    // Memory diagnostics
    MemoryAllocationFailed = 0x00020001,
    MemoryLimitExceeded = 0x00020002,

    // Module diagnostics
    ModuleLoadFailed = 0x00030001,
    ModuleUnloadFailed = 0x00030002,
    ModuleNotFound = 0x00030003,

    // Plugin diagnostics
    PluginLoadFailed = 0x00040001,
    PluginInitializationFailed = 0x00040002,

    // Process diagnostics
    ProcessCreationFailed = 0x00050001,
    ProcessTerminationFailed = 0x00050002,

    // Resource diagnostics
    ResourceNotFound = 0x00060001,
    ResourceUnavailable = 0x00060002,

    // System diagnostics
    SystemCallFailed = 0x00070001,
    PermissionDenied = 0x00070002,
    FileOperationFailed = 0x00070003
};

[[nodiscard]]
constexpr std::string_view diagnosticCodeName(
    DiagnosticCode code
) noexcept {

    switch (code) {

        case DiagnosticCode::Unknown:
            return "UNKNOWN";

        case DiagnosticCode::GeneralFailure:
            return "GENERAL_FAILURE";

        case DiagnosticCode::InvalidArgument:
            return "INVALID_ARGUMENT";

        case DiagnosticCode::InvalidState:
            return "INVALID_STATE";

        case DiagnosticCode::OperationFailed:
            return "OPERATION_FAILED";

        case DiagnosticCode::RuntimeInitializationFailed:
            return "RUNTIME_INITIALIZATION_FAILED";

        case DiagnosticCode::RuntimeShutdownFailed:
            return "RUNTIME_SHUTDOWN_FAILED";

        case DiagnosticCode::RuntimeConfigurationFailed:
            return "RUNTIME_CONFIGURATION_FAILED";

        case DiagnosticCode::MemoryAllocationFailed:
            return "MEMORY_ALLOCATION_FAILED";

        case DiagnosticCode::MemoryLimitExceeded:
            return "MEMORY_LIMIT_EXCEEDED";

        case DiagnosticCode::ModuleLoadFailed:
            return "MODULE_LOAD_FAILED";

        case DiagnosticCode::ModuleUnloadFailed:
            return "MODULE_UNLOAD_FAILED";

        case DiagnosticCode::ModuleNotFound:
            return "MODULE_NOT_FOUND";

        case DiagnosticCode::PluginLoadFailed:
            return "PLUGIN_LOAD_FAILED";

        case DiagnosticCode::PluginInitializationFailed:
            return "PLUGIN_INITIALIZATION_FAILED";

        case DiagnosticCode::ProcessCreationFailed:
            return "PROCESS_CREATION_FAILED";

        case DiagnosticCode::ProcessTerminationFailed:
            return "PROCESS_TERMINATION_FAILED";

        case DiagnosticCode::ResourceNotFound:
            return "RESOURCE_NOT_FOUND";

        case DiagnosticCode::ResourceUnavailable:
            return "RESOURCE_UNAVAILABLE";

        case DiagnosticCode::SystemCallFailed:
            return "SYSTEM_CALL_FAILED";

        case DiagnosticCode::PermissionDenied:
            return "PERMISSION_DENIED";

        case DiagnosticCode::FileOperationFailed:
            return "FILE_OPERATION_FAILED";
    }

    return "UNRECOGNIZED_DIAGNOSTIC_CODE";
}

} // namespace nexora::diagnostics
