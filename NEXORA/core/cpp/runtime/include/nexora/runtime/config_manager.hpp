
#pragma once

#include <nexora/runtime/config.hpp>
#include <nexora/runtime/config_validator.hpp>
#include <nexora/runtime/status.hpp>

#include <string>

namespace nexora::runtime {

class ConfigManager final {
public:
    ConfigManager() = default;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    [[nodiscard]]
    Status load(const RuntimeConfig& config) {
        const Status validation =
            ConfigValidator::validate(config);

        if (!validation) {
            return validation;
        }

        // Store an owned copy of the instance name.
        instance_name_.assign(
            config.instance_name.data(),
            config.instance_name.size()
        );

        config_ = config;
        config_.instance_name = instance_name_;

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
        loaded_ = false;
        config_ = RuntimeConfig{};
        instance_name_.clear();
    }

private:
    std::string instance_name_{};
    RuntimeConfig config_{};
    bool loaded_{false};
};

} // namespace nexora::runtime
