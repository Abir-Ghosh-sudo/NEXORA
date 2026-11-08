
#pragma once

// Diagnostic definitions
#include <nexora/diagnostics/diagnostic_severity.hpp>
#include <nexora/diagnostics/diagnostic_code.hpp>
#include <nexora/diagnostics/diagnostic_location.hpp>

// Diagnostic events and results
#include <nexora/diagnostics/diagnostic_event.hpp>
#include <nexora/diagnostics/diagnostic_result.hpp>
#include <nexora/diagnostics/diagnostic_event_factory.hpp>

// Context and processing
#include <nexora/diagnostics/diagnostic_context.hpp>
#include <nexora/diagnostics/diagnostic_filter.hpp>
#include <nexora/diagnostics/diagnostic_statistics.hpp>
#include <nexora/diagnostics/diagnostic_collector.hpp>
#include <nexora/diagnostics/diagnostic_manager.hpp>

// Reporting and formatting
#include <nexora/diagnostics/diagnostic_reporter.hpp>
#include <nexora/diagnostics/diagnostic_formatter.hpp>
#include <nexora/diagnostics/json_diagnostic_formatter.hpp>

// Output sinks
#include <nexora/diagnostics/diagnostic_sink.hpp>
#include <nexora/diagnostics/console_diagnostic_sink.hpp>
#include <nexora/diagnostics/file_diagnostic_sink.hpp>
#include <nexora/diagnostics/json_file_diagnostic_sink.hpp>
#include <nexora/diagnostics/multi_diagnostic_sink.hpp>

// Event dispatching
#include <nexora/diagnostics/diagnostic_dispatcher.hpp>

// Diagnostic macros
#include <nexora/diagnostics/diagnostic_macros.hpp>
