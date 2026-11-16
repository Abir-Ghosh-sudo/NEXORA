
#pragma once

#include "config_value.hpp"

#include <chrono>
#include <string>
#include <utility>

namespace nexora::configuration
{

class ConfigEntry
{
public:
    ConfigEntry() = default;

    explicit ConfigEntry(
        std::string key,
        ConfigValue value,
        std::string description = {}
    )
        : key_(std::move(key)),
          value_(std::move(value)),
          description_(std::move(description)),
          createdAt_(std::chrono::system_clock::now()),
          updatedAt_(createdAt_)
    {
    }

    [[nodiscard]]
    const std::string& key() const noexcept
    {
        return key_;
    }

    [[nodiscard]]
    const ConfigValue& value() const noexcept
    {
        return value_;
    }

    [[nodiscard]]
    ConfigValue& value() noexcept
    {
        return value_;
    }

    [[nodiscard]]
    const std::string& description() const noexcept
    {
        return description_;
    }

    [[nodiscard]]
    std::chrono::system_clock::time_point createdAt() const noexcept
    {
        return createdAt_;
    }

    [[nodiscard]]
    std::chrono::system_clock::time_point updatedAt() const noexcept
    {
        return updatedAt_;
    }

    void setValue(ConfigValue value)
    {
        value_ = std::move(value);
        updatedAt_ = std::chrono::system_clock::now();
    }

    void setDescription(std::string description)
    {
        description_ = std::move(description);
        updatedAt_ = std::chrono::system_clock::now();
    }

private:
    std::string key_;
    ConfigValue value_;
    std::string description_;

    std::chrono::system_clock::time_point createdAt_{};
    std::chrono::system_clock::time_point updatedAt_{};
};

} // namespace nexora::configuration
