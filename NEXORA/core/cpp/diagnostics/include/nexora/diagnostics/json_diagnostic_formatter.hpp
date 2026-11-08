
#pragma once

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

#include <nexora/diagnostics/diagnostic_event.hpp>

namespace nexora::diagnostics {

class JsonDiagnosticFormatter final {
public:

    [[nodiscard]]
    static std::string escape(const std::string& value) {
        std::ostringstream output;

        for (const unsigned char character : value) {
            switch (character) {
                case '"':
                    output << "\\\"";
                    break;

                case '\\':
                    output << "\\\\";
                    break;

                case '\b':
                    output << "\\b";
                    break;

                case '\f':
                    output << "\\f";
                    break;

                case '\n':
                    output << "\\n";
                    break;

                case '\r':
                    output << "\\r";
                    break;

                case '\t':
                    output << "\\t";
                    break;

                default:
                    if (character < 0x20) {
                        output << "\\u"
                               << std::hex
                               << std::setw(4)
                               << std::setfill('0')
                               << static_cast<unsigned int>(character)
                               << std::dec;
                    } else {
                        output << static_cast<char>(character);
                    }
                    break;
            }
        }

        return output.str();
    }

    [[nodiscard]]
    static std::string format(
        const DiagnosticEvent& event
    ) {
        const auto timestamp_ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                event.timestamp.time_since_epoch()
            ).count();

        std::ostringstream output;

        output
            << "{"
            << "\"severity\":\""
            << diagnosticSeverityName(event.severity)
            << "\","
            << "\"code\":\""
            << diagnosticCodeName(event.code)
            << "\","
            << "\"code_value\":"
            << static_cast<std::uint32_t>(event.code)
            << ","
            << "\"message\":\""
            << escape(event.message)
            << "\","
            << "\"timestamp_ms\":"
            << timestamp_ms
            << ","
            << "\"thread_id\":\""
            << escape(
                [&event]() {
                    std::ostringstream thread;
                    thread << event.thread_id;
                    return thread.str();
                }()
            )
            << "\","
            << "\"location\":{"
            << "\"file\":\""
            << escape(std::string(event.location.file))
            << "\","
            << "\"function\":\""
            << escape(std::string(event.location.function))
            << "\","
            << "\"line\":"
            << event.location.line
            << ","
            << "\"column\":"
            << event.location.column
            << "}"
            << "}";

        return output.str();
    }
};

} // namespace nexora::diagnostics
