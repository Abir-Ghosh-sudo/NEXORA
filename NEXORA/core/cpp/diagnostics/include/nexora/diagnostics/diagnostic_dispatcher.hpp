
#pragma once

#include <nexora/diagnostics/diagnostic_event.hpp>
#include <nexora/diagnostics/diagnostic_manager.hpp>
#include <nexora/diagnostics/diagnostic_sink.hpp>

namespace nexora::diagnostics {

class DiagnosticDispatcher final {
public:

    DiagnosticDispatcher(
        DiagnosticManager& manager,
        DiagnosticSink& sink
    ) noexcept
        : manager_(manager),
          sink_(sink) {}

    DiagnosticDispatcher(
        const DiagnosticDispatcher&
    ) = delete;

    DiagnosticDispatcher& operator=(
        const DiagnosticDispatcher&
    ) = delete;

    void dispatch(const DiagnosticEvent& event) {
        manager_.record(event);
        sink_.write(event);
    }

    void flush() {
        sink_.flush();
    }

private:

    DiagnosticManager& manager_;
    DiagnosticSink& sink_;
};

} // namespace nexora::diagnostics
