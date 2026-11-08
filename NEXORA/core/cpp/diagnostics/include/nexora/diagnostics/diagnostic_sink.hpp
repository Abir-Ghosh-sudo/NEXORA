
#pragma once

#include <nexora/diagnostics/diagnostic_event.hpp>

namespace nexora::diagnostics {

class DiagnosticSink {
public:

    virtual ~DiagnosticSink() = default;

    DiagnosticSink() = default;

    DiagnosticSink(const DiagnosticSink&) = default;
    DiagnosticSink& operator=(const DiagnosticSink&) = default;

    DiagnosticSink(DiagnosticSink&&) noexcept = default;
    DiagnosticSink& operator=(DiagnosticSink&&) noexcept = default;

    virtual void write(
        const DiagnosticEvent& event
    ) = 0;

    virtual void flush() = 0;
};

} // namespace nexora::diagnostics
