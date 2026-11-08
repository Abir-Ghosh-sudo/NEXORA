
#pragma once

#include <chrono>
#include <string>
#include <thread>
#include <utility>

#include <nexora/diagnostics/diagnostic_code.hpp>
#include <nexora/diagnostics/diagnostic_event.hpp>
#include <nexora/diagnostics/diagnostic_location.hpp>
#include <nexora/diagnostics/diagnostic_severity.hpp>

namespace nexora::diagnostics {

class DiagnosticEventFactory final {
public:

    [[nodiscard]]
    static DiagnosticEvent create(
        DiagnosticSeverity severity,
        DiagnosticCode code,
        std::string message,
        DiagnosticLocation location = {}
    ) {
        DiagnosticEvent event{};

        event.severity = severity;
        event.code = code;
        event.message = std::move(message);
        event.location = location;

        event.timestamp =
            DiagnosticEvent::Clock::now();

        event.thread_id =
            std::this_thread::get_id();

        return event;
    }
};

} // namespace nexora::diagnostics
