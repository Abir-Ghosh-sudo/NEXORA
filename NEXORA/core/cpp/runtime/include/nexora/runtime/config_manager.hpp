
#pragma once

#include <nexora/runtime/config.hpp>
#include <nexora/runtime/config_validator.hpp>
#include <nexora/runtime/status.hpp>

namespace nexora::runtime {

class ConfigManager final {
public:
    ConfigManager() = default;

    [[nodiscard]]
    Status load(const RuntimeConfig& config) noexcept {
        const Status validation =
            ConfigValidator::validate(config);

        if (!validation) {
            return validation;
        }

        config_ = config;
        loaded_ = true;

        return Status::success();
    }

    [[nodiscard]]
    bool isLoaded() const noexcept {
        return loaded_;
    }

    [[nodiscard]]
    const RuntimeConfig* get() const noexcept {
        return loaded_ ? &config_ : nullptr;
    }

    void reset() noexcept {
        config_ = RuntimeConfig{};
        loaded_ = false;
    }

private:
    RuntimeConfig config_{};
    bool loaded_{false};
};

} // namespace nexora::runtime
