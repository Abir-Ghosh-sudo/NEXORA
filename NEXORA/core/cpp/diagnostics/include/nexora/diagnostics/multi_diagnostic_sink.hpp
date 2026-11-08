
#pragma once

#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>
#include <vector>

#include <nexora/diagnostics/diagnostic_event.hpp>
#include <nexora/diagnostics/diagnostic_sink.hpp>

namespace nexora::diagnostics {

class MultiDiagnosticSink final : public DiagnosticSink {
public:

    MultiDiagnosticSink() = default;

    MultiDiagnosticSink(
        const MultiDiagnosticSink&
    ) = delete;

    MultiDiagnosticSink& operator=(
        const MultiDiagnosticSink&
    ) = delete;

    void addSink(std::shared_ptr<DiagnosticSink> sink) {
        if (!sink) {
            throw std::invalid_argument(
                "Diagnostic sink cannot be null"
            );
        }

        std::lock_guard<std::mutex> lock(mutex_);
        sinks_.push_back(std::move(sink));
    }

    void write(
        const DiagnosticEvent& event
    ) override {
        std::lock_guard<std::mutex> lock(mutex_);

        for (const auto& sink : sinks_) {
            sink->write(event);
        }
    }

    void flush() override {
        std::lock_guard<std::mutex> lock(mutex_);

        for (const auto& sink : sinks_) {
            sink->flush();
        }
    }

    [[nodiscard]]
    std::size_t sinkCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return sinks_.size();
    }

    void clearSinks() {
        std::lock_guard<std::mutex> lock(mutex_);
        sinks_.clear();
    }

private:

    mutable std::mutex mutex_{};

    std::vector<std::shared_ptr<DiagnosticSink>> sinks_{};
};

} // namespace nexora::diagnostics
