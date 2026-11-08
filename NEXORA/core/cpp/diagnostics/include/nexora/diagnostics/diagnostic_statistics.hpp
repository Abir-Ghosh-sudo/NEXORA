
#pragma once

#include <array>
#include <cstddef>

#include <nexora/diagnostics/diagnostic_event.hpp>

namespace nexora::diagnostics {

class DiagnosticStatistics final {
public:

    static constexpr std::size_t SeverityCount = 5;

    void record(const DiagnosticEvent& event) noexcept {
        if (!isValidDiagnosticSeverity(event.severity)) {
            return;
        }

        const auto index =
            static_cast<std::size_t>(event.severity);

        ++severity_counts_[index];
        ++total_count_;
    }

    [[nodiscard]]
    std::size_t count(
        DiagnosticSeverity severity
    ) const noexcept {
        if (!isValidDiagnosticSeverity(severity)) {
            return 0;
        }

        return severity_counts_[
            static_cast<std::size_t>(severity)
        ];
    }

    [[nodiscard]]
    std::size_t totalCount() const noexcept {
        return total_count_;
    }

    void reset() noexcept {
        severity_counts_.fill(0);
        total_count_ = 0;
    }

private:

    std::array<std::size_t, SeverityCount> severity_counts_{};

    std::size_t total_count_{0};
};

} // namespace nexora::diagnostics
