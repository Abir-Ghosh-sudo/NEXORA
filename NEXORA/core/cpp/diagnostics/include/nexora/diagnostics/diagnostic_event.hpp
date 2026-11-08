
#pragma once

#include <chrono>
#include <string>
#include <thread>

#include <nexora/diagnostics/diagnostic_code.hpp>
#include <nexora/diagnostics/diagnostic_location.hpp>
#include <nexora/diagnostics/diagnostic_severity.hpp>

namespace nexora::diagnostics {

struct DiagnosticEvent final {

    using Clock = std::chrono::system_clock;
    using Timestamp = Clock::time_point;

    DiagnosticSeverity severity{
        DiagnosticSeverity::Info
    };

    DiagnosticCode code{
        DiagnosticCode::Unknown
    };

    std::string message{};

    DiagnosticLocation location{};

    Timestamp timestamp{};

    std::thread::id thread_id{};

    [[nodiscard]]
    bool isValid() const noexcept {
        return isValidDiagnosticSeverity(severity) &&
               !message.empty();
    }
};

} // namespace nexora::diagnostics
