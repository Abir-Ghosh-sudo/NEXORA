
#pragma once

#include <string>

#include <nexora/diagnostics/diagnostic_event.hpp>

namespace nexora::diagnostics {

class DiagnosticReporter {
public:

    virtual ~DiagnosticReporter() = default;

    DiagnosticReporter() = default;

    DiagnosticReporter(const DiagnosticReporter&) = default;
    DiagnosticReporter& operator=(const DiagnosticReporter&) = default;

    DiagnosticReporter(DiagnosticReporter&&) noexcept = default;
    DiagnosticReporter& operator=(DiagnosticReporter&&) noexcept = default;

    [[nodiscard]]
    virtual std::string report(
        const DiagnosticEvent& event
    ) const = 0;
};

} // namespace nexora::diagnostics
