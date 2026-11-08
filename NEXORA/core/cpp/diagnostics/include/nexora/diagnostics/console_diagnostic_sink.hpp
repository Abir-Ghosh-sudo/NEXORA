
#pragma once

#include <iostream>
#include <mutex>
#include <ostream>

#include <nexora/diagnostics/diagnostic_event.hpp>
#include <nexora/diagnostics/diagnostic_formatter.hpp>
#include <nexora/diagnostics/diagnostic_sink.hpp>

namespace nexora::diagnostics {

class ConsoleDiagnosticSink final : public DiagnosticSink {
public:

    explicit ConsoleDiagnosticSink(
        std::ostream& output = std::clog
    ) noexcept
        : output_(output) {}

    ConsoleDiagnosticSink(
        const ConsoleDiagnosticSink&
    ) = delete;

    ConsoleDiagnosticSink& operator=(
        const ConsoleDiagnosticSink&
    ) = delete;

    void write(
        const DiagnosticEvent& event
    ) override {
        const std::string formatted =
            DiagnosticFormatter::format(event);

        std::lock_guard<std::mutex> lock(mutex_);

        output_ << formatted << '\n';
    }

    void flush() override {
        std::lock_guard<std::mutex> lock(mutex_);
        output_.flush();
    }

private:

    std::ostream& output_;
    std::mutex mutex_{};
};

} // namespace nexora::diagnostics
