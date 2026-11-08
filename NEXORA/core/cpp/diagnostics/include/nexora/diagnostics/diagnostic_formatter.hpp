
#pragma once

#include <chrono>
#include <cstdint>
#include <sstream>
#include <string>

#include <nexora/diagnostics/diagnostic_event.hpp>

namespace nexora::diagnostics {

class DiagnosticFormatter final {
public:

    [[nodiscard]]
    static std::string format(
        const DiagnosticEvent& event
    ) {
        std::ostringstream output;

        const auto timestamp_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                event.timestamp.time_since_epoch()
            ).count();

        output
            << "[TIME: " << timestamp_ms << "ms]"
            << " [SEVERITY: "
            << diagnosticSeverityName(event.severity) << "]"
            << " [CODE: "
            << diagnosticCodeName(event.code) << "]"
            << " [CODE_VALUE: "
            << static_cast<std::uint32_t>(event.code) << "]"
            << " [THREAD: " << event.thread_id << "]"
            << " Message: " << event.message;

        if (!event.location.file.empty()) {
            output
                << " [LOCATION: "
                << event.location.file;

            if (event.location.line > 0) {
                output << ":" << event.location.line;
            }

            if (event.location.column > 0) {
                output << ":" << event.location.column;
            }

            if (!event.location.function.empty()) {
                output << " in " << event.location.function;
            }

            output << "]";
        }

        return output.str();
    }
};

} // namespace nexora::diagnostics
