
#pragma once

#include <nexora/runtime/version.hpp>

#include <string_view>

namespace nexora::runtime {

struct RuntimeConfig final {
    std::string_view instance_name{"NEXORA"};

    Version version{};

    bool enable_diagnostics{true};

    bool enable_plugins{true};

    bool enable_ai_services{false};
};

} // namespace nexora::runtime
