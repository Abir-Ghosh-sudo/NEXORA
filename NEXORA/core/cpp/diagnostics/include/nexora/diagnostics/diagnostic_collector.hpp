
#pragma once

#include <cstddef>
#include <mutex>
#include <vector>

#include <nexora/diagnostics/diagnostic_event.hpp>

namespace nexora::diagnostics {

class DiagnosticCollector final {
public:

    DiagnosticCollector() = default;

    DiagnosticCollector(const DiagnosticCollector&) = delete;
    DiagnosticCollector& operator=(const DiagnosticCollector&) = delete;

    void collect(DiagnosticEvent event) {
        std::lock_guard<std::mutex> lock(mutex_);
        events_.push_back(std::move(event));
    }

    [[nodiscard]]
    std::vector<DiagnosticEvent> snapshot() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return events_;
    }

    [[nodiscard]]
    std::size_t size() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return events_.size();
    }

    [[nodiscard]]
    bool empty() const noexcept {
        std::lock_guard<std::mutex> lock(mutex_);
        return events_.empty();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        events_.clear();
    }

private:

    mutable std::mutex mutex_{};
    std::vector<DiagnosticEvent> events_{};
};

} // namespace nexora::diagnostics
