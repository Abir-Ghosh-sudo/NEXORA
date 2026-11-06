
#pragma once

#include <nexora/runtime/config.hpp>
#include <nexora/runtime/status.hpp>

namespace nexora::runtime {

class ConfigValidator final {
public:
    [[nodiscard]]
    static constexpr Status validate(
        const RuntimeConfig& config
    ) noexcept {
        if (config.instance_name.empty()) {
            return Status::failure(makeError(
                ErrorCode::ConfigurationInvalid,
                ErrorCategory::Configuration,
                ErrorSeverity::Error,
                "Runtime instance name cannot be empty."
            ));
        }

        if (config.version.major == 0 &&
            config.version.minor == 0 &&
            config.version.patch == 0) {
            return Status::failure(makeError(
                ErrorCode::ConfigurationInvalid,
                ErrorCategory::Configuration,
                ErrorSeverity::Error,
                "Runtime version cannot be 0.0.0."
            ));
        }

        return Status::success();
    }
};

} // namespace nexora::runtime
