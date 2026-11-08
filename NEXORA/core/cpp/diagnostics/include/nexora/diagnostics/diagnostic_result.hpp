
#pragma once

#include <optional>
#include <utility>

#include <nexora/diagnostics/diagnostic_event.hpp>

namespace nexora::diagnostics {

class DiagnosticResult final {
public:

    [[nodiscard]]
    static DiagnosticResult success() noexcept {
        return DiagnosticResult{};
    }

    [[nodiscard]]
    static DiagnosticResult failure(
        DiagnosticEvent event
    ) {
        return DiagnosticResult{
            std::move(event)
        };
    }

    [[nodiscard]]
    bool succeeded() const noexcept {
        return !event_.has_value();
    }

    [[nodiscard]]
    bool failed() const noexcept {
        return event_.has_value();
    }

    [[nodiscard]]
    const std::optional<DiagnosticEvent>& event() const noexcept {
        return event_;
    }

private:

    DiagnosticResult() = default;

    explicit DiagnosticResult(DiagnosticEvent event)
        : event_(std::move(event)) {}

    std::optional<DiagnosticEvent> event_{};
};

} // namespace nexora::diagnostics
