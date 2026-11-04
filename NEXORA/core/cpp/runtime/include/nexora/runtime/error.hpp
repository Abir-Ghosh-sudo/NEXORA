
#pragma once

#include <cstdint>
#include <string_view>

namespace nexora::runtime {

// ============================================================
// Error Categories
// ============================================================

enum class ErrorCategory : std::uint8_t {
    None = 0,
    Runtime,
    Configuration,
    Initialization,
    Lifecycle,
    Resource,
    Module,
    Plugin,
    Security,
    Platform,
    Internal
};

// ============================================================
// Error Severity
// ============================================================

enum class ErrorSeverity : std::uint8_t {
    Trace = 0,
    Warning,
    Error,
    Critical,
    Fatal
};

// ============================================================
// Runtime Error Codes
// ============================================================

enum class ErrorCode : std::uint32_t {

    None = 0,

    // Runtime errors: 1000 - 1099
    RuntimeNotInitialized = 1000,
    RuntimeAlreadyInitialized,
    RuntimeShutdownFailed,
    RuntimeInvalidState,
    RuntimeInitializationFailed,

    // Configuration errors: 1100 - 1199
    ConfigurationNotFound = 1100,
    ConfigurationInvalid,
    ConfigurationLoadFailed,

    // Resource errors: 1200 - 1299
    ResourceUnavailable = 1200,
    ResourceAllocationFailed,
    ResourceLimitExceeded,

    // Module errors: 1300 - 1399
    ModuleNotFound = 1300,
    ModuleAlreadyLoaded,
    ModuleLoadFailed,
    ModuleInitializationFailed,

    // Plugin errors: 1400 - 1499
    PluginNotFound = 1400,
    PluginLoadFailed,
    PluginIncompatible,

    // Security errors: 1500 - 1599
    PermissionDenied = 1500,
    SecurityViolation,
    AccessRestricted,

    // Platform errors: 1600 - 1699
    UnsupportedPlatform = 1600,
    PlatformOperationFailed,

    // Internal errors: 1900 - 1999
    InternalError = 1900,
    UnknownError
};

// ============================================================
// Error Structure
// ============================================================

struct Error final {

    ErrorCode code{ErrorCode::None};

    ErrorCategory category{ErrorCategory::None};

    ErrorSeverity severity{ErrorSeverity::Error};

    std::string_view message{};

    [[nodiscard]]
    constexpr bool isOk() const noexcept {
        return code == ErrorCode::None;
    }

    [[nodiscard]]
    constexpr explicit operator bool() const noexcept {
        return !isOk();
    }
};

// ============================================================
// Error Factory
// ============================================================

[[nodiscard]]
constexpr Error makeError(
    ErrorCode code,
    ErrorCategory category,
    ErrorSeverity severity,
    std::string_view message
) noexcept {

    return Error{
        code,
        category,
        severity,
        message
    };
}

// ============================================================
// Error Code Conversion
// ============================================================

[[nodiscard]]
constexpr std::string_view errorCodeName(
    ErrorCode code
) noexcept {

    switch (code) {

        case ErrorCode::None:
            return "None";

        case ErrorCode::RuntimeNotInitialized:
            return "RuntimeNotInitialized";

        case ErrorCode::RuntimeAlreadyInitialized:
            return "RuntimeAlreadyInitialized";

        case ErrorCode::RuntimeShutdownFailed:
            return "RuntimeShutdownFailed";

        case ErrorCode::RuntimeInvalidState:
            return "RuntimeInvalidState";

        case ErrorCode::RuntimeInitializationFailed:
            return "RuntimeInitializationFailed";

        case ErrorCode::ConfigurationNotFound:
            return "ConfigurationNotFound";

        case ErrorCode::ConfigurationInvalid:
            return "ConfigurationInvalid";

        case ErrorCode::ConfigurationLoadFailed:
            return "ConfigurationLoadFailed";

        case ErrorCode::ResourceUnavailable:
            return "ResourceUnavailable";

        case ErrorCode::ResourceAllocationFailed:
            return "ResourceAllocationFailed";

        case ErrorCode::ResourceLimitExceeded:
            return "ResourceLimitExceeded";

        case ErrorCode::ModuleNotFound:
            return "ModuleNotFound";

        case ErrorCode::ModuleAlreadyLoaded:
            return "ModuleAlreadyLoaded";

        case ErrorCode::ModuleLoadFailed:
            return "ModuleLoadFailed";

        case ErrorCode::ModuleInitializationFailed:
            return "ModuleInitializationFailed";

        case ErrorCode::PluginNotFound:
            return "PluginNotFound";

        case ErrorCode::PluginLoadFailed:
            return "PluginLoadFailed";

        case ErrorCode::PluginIncompatible:
            return "PluginIncompatible";

        case ErrorCode::PermissionDenied:
            return "PermissionDenied";

        case ErrorCode::SecurityViolation:
            return "SecurityViolation";

        case ErrorCode::AccessRestricted:
            return "AccessRestricted";

        case ErrorCode::UnsupportedPlatform:
            return "UnsupportedPlatform";

        case ErrorCode::PlatformOperationFailed:
            return "PlatformOperationFailed";

        case ErrorCode::InternalError:
            return "InternalError";

        case ErrorCode::UnknownError:
            return "UnknownError";

        default:
            return "UnrecognizedError";
    }
}

// ============================================================
// Severity Conversion
// ============================================================

[[nodiscard]]
constexpr std::string_view severityName(
    ErrorSeverity severity
) noexcept {

    switch (severity) {

        case ErrorSeverity::Trace:
            return "Trace";

        case ErrorSeverity::Warning:
            return "Warning";

        case ErrorSeverity::Error:
            return "Error";

        case ErrorSeverity::Critical:
            return "Critical";

        case ErrorSeverity::Fatal:
            return "Fatal";

        default:
            return "Unknown";
    }
}

// ============================================================
// Category Conversion
// ============================================================

[[nodiscard]]
constexpr std::string_view categoryName(
    ErrorCategory category
) noexcept {

    switch (category) {

        case ErrorCategory::None:
            return "None";

        case ErrorCategory::Runtime:
            return "Runtime";

        case ErrorCategory::Configuration:
            return "Configuration";

        case ErrorCategory::Initialization:
            return "Initialization";

        case ErrorCategory::Lifecycle:
            return "Lifecycle";

        case ErrorCategory::Resource:
            return "Resource";

        case ErrorCategory::Module:
            return "Module";

        case ErrorCategory::Plugin:
            return "Plugin";

        case ErrorCategory::Security:
            return "Security";

        case ErrorCategory::Platform:
            return "Platform";

        case ErrorCategory::Internal:
            return "Internal";

        default:
            return "Unknown";
    }
}

} // namespace nexora::runtime
