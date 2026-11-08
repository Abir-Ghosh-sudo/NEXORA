
#pragma once

#include <string_view>

namespace nexora::diagnostics {

enum class DiagnosticSeverity : unsigned char {
    Trace = 0,
    Info,
    Warning,
    Error,
    Critical
};

[[nodiscard]]
constexpr std::string_view diagnosticSeverityName(
    DiagnosticSeverity severity
) noexcept {
    switch (severity) {
        case DiagnosticSeverity::Trace:
            return "TRACE";

        case DiagnosticSeverity::Info:
            return "INFO";

        case DiagnosticSeverity::Warning:
            return "WARNING";

        case DiagnosticSeverity::Error:
            return "ERROR";

        case DiagnosticSeverity::Critical:
            return "CRITICAL";
    }

    return "UNKNOWN";
}

[[nodiscard]]
constexpr bool isValidDiagnosticSeverity(
    DiagnosticSeverity severity
) noexcept {
    return severity >= DiagnosticSeverity::Trace &&
           severity <= DiagnosticSeverity::Critical;
}

} // namespace nexora::diagnostics
