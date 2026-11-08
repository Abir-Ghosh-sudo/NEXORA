
#pragma once

#include <fstream>
#include <mutex>
#include <string>

#include <nexora/diagnostics/diagnostic_event.hpp>
#include <nexora/diagnostics/diagnostic_formatter.hpp>
#include <nexora/diagnostics/diagnostic_sink.hpp>

namespace nexora::diagnostics {

class FileDiagnosticSink final : public DiagnosticSink {
public:

    explicit FileDiagnosticSink(
        const std::string& filePath
    )
        : output_(filePath, std::ios::out | std::ios::app) {}

    FileDiagnosticSink(
        const FileDiagnosticSink&
    ) = delete;

    FileDiagnosticSink& operator=(
        const FileDiagnosticSink&
    ) = delete;

    [[nodiscard]]
    bool isOpen() const noexcept {
        return output_.is_open() && output_.good();
    }

    void write(
        const DiagnosticEvent& event
    ) override {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!output_.is_open()) {
            return;
        }

        output_
            << DiagnosticFormatter::format(event)
            << '\n';
    }

    void flush() override {
        std::lock_guard<std::mutex> lock(mutex_);

        if (output_.is_open()) {
            output_.flush();
        }
    }

private:

    std::ofstream output_;
    std::mutex mutex_{};
};

} // namespace nexora::diagnostics
