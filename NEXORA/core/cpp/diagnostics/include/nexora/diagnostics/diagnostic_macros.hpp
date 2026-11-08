
#pragma once

#include <nexora/diagnostics/diagnostic_location.hpp>

#define NEXORA_DIAGNOSTIC_LOCATION \
    ::nexora::diagnostics::DiagnosticLocation{ \
        __FILE__, \
        __func__, \
        static_cast<std::uint_least32_t>(__LINE__), \
        0 \
    }

#define NEXORA_DIAGNOSTIC_LOCATION_AT(column_number) \
    ::nexora::diagnostics::DiagnosticLocation{ \
        __FILE__, \
        __func__, \
        static_cast<std::uint_least32_t>(__LINE__), \
        static_cast<std::uint_least32_t>(column_number) \
    }
