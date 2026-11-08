
#pragma once

#include <cstddef>
#include <vector>

#include <nexora/diagnostics/diagnostic_collector.hpp>
#include <nexora/diagnostics/diagnostic_event.hpp>
#include <nexora/diagnostics/diagnostic_filter.hpp>
#include <nexora/diagnostics/diagnostic_statistics.hpp>

namespace nexora::diagnostics {

class DiagnosticManager final {
public:

    DiagnosticManager() = default;

    DiagnosticManager(const DiagnosticManager&) = delete;
    DiagnosticManager& operator=(const DiagnosticManager&) = delete;

    void record(DiagnosticEvent event) {
        statistics_.record(event);
        collector_.collect(std::move(event));
    }

    [[nodiscard]]
    std::vector<DiagnosticEvent> events() const {
        return collector_.snapshot();
    }

    [[nodiscard]]
    std::vector<DiagnosticEvent> filteredEvents() const {
        return filter_.filter(collector_.snapshot());
    }

    void setMinimumSeverity(
        DiagnosticSeverity severity
    ) noexcept {
        filter_.setMinimumSeverity(severity);
    }

    [[nodiscard]]
    std::size_t eventCount() const {
        return collector_.size();
    }

    [[nodiscard]]
    std::size_t count(
        DiagnosticSeverity severity
    ) const noexcept {
        return statistics_.count(severity);
    }

    [[nodiscard]]
    std::size_t totalCount() const noexcept {
        return statistics_.totalCount();
    }

    void clear() {
        collector_.clear();
        statistics_.reset();
    }

private:

    DiagnosticCollector collector_{};
    DiagnosticFilter filter_{};
    DiagnosticStatistics statistics_{};
};

} // namespace nexora::diagnostics
