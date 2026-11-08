
#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

#include <nexora/diagnostics/diagnostic_event.hpp>

namespace nexora::diagnostics {

class DiagnosticFilter final {
public:

    explicit DiagnosticFilter(
        DiagnosticSeverity minimumSeverity =
            DiagnosticSeverity::Trace
    ) noexcept
        : minimum_severity_(minimumSeverity) {}

    void setMinimumSeverity(
        DiagnosticSeverity severity
    ) noexcept {
        minimum_severity_ = severity;
    }

    [[nodiscard]]
    DiagnosticSeverity minimumSeverity() const noexcept {
        return minimum_severity_;
    }

    [[nodiscard]]
    bool matches(
        const DiagnosticEvent& event
    ) const noexcept {
        return isValidDiagnosticSeverity(event.severity) &&
               isValidDiagnosticSeverity(minimum_severity_) &&
               event.severity >= minimum_severity_;
    }

    [[nodiscard]]
    std::vector<DiagnosticEvent> filter(
        const std::vector<DiagnosticEvent>& events
    ) const {
        std::vector<DiagnosticEvent> result;

        std::copy_if(
            events.begin(),
            events.end(),
            std::back_inserter(result),
            [this](const DiagnosticEvent& event) {
                return matches(event);
            }
        );

        return result;
    }

private:

    DiagnosticSeverity minimum_severity_;
};

} // namespace nexora::diagnostics
