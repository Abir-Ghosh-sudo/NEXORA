
#pragma once

#include "config_error.hpp"
#include "config_result.hpp"
#include "config_value.hpp"

#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace nexora::configuration
{

class ConfigSection
{
public:
    explicit ConfigSection(std::string name)
        : name_(std::move(name))
    {
    }

    ConfigSection(const ConfigSection&) = delete;
    ConfigSection& operator=(const ConfigSection&) = delete;

    [[nodiscard]]
    const std::string& name() const noexcept
    {
        return name_;
    }

    [[nodiscard]]
    ConfigResult<ConfigValue> get(const std::string& key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const auto iterator = values_.find(key);

        if (iterator == values_.end())
        {
            return ConfigError{
                ConfigErrorCode::NotFound,
                "Configuration key not found in section: " + key
            };
        }

        return iterator->second;
    }

    [[nodiscard]]
    ConfigResult<bool> set(
        const std::string& key,
        ConfigValue value
    )
    {
        if (key.empty())
        {
            return ConfigError{
                ConfigErrorCode::InvalidValue,
                "Configuration key cannot be empty."
            };
        }

        std::lock_guard<std::mutex> lock(mutex_);

        values_.insert_or_assign(
            key,
            std::move(value)
        );

        return true;
    }

    [[nodiscard]]
    bool contains(const std::string& key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return values_.find(key) != values_.end();
    }

    [[nodiscard]]
    bool remove(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return values_.erase(key) > 0;
    }

    [[nodiscard]]
    std::vector<std::string> keys() const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::vector<std::string> result;
        result.reserve(values_.size());

        for (const auto& [key, value] : values_)
        {
            result.push_back(key);
        }

        return result;
    }

    [[nodiscard]]
    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return values_.size();
    }

    [[nodiscard]]
    bool empty() const
    {
        return size() == 0;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        values_.clear();
    }

private:
    std::string name_;

    mutable std::mutex mutex_;

    std::unordered_map<std::string, ConfigValue> values_;
};

} // namespace nexora::configuration
