
#pragma once

#include <nexora/runtime/config.hpp>

namespace nexora::runtime {

class ConfigDefaults final {
public:
    [[nodiscard]]
    static constexpr RuntimeConfig create() noexcept {
        return RuntimeConfig{
            "NEXORA",
            Version{1, 0, 0},
            true,
            true,
            false
        };
    }

    ConfigDefaults() = delete;
};

} // namespace nexora::runtime
