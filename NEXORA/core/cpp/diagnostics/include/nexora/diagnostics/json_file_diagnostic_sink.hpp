
#pragma once

#include <fstream>
#include <mutex>
#include <string>

#include <nexora/diagnostics/diagnostic_event.hpp>
#include <nexora/diagnostics/diagnostic_sink.hpp>
#include <nexora/diagnostics/json_diagnostic_formatter.hpp>

namespace nexora::diagnostics {

class JsonFileDiagnosticSink final : public DiagnosticSink {
public:

    explicit JsonFileDiagnosticSink(
        const std::string& filePath
    )
        : output_(filePath, std::ios::out | std::ios::app) {}

    JsonFileDiagnosticSink(
        const JsonFileDiagnosticSink&
    ) = delete;

    JsonFileDiagnosticSink& operator=(
        const JsonFileDiagnosticSink&
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
            << JsonDiagnosticFormatter::format(event)
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
