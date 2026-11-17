
#pragma once

#include "config_defaults.hpp"
#include "config_entry.hpp"
#include "config_error.hpp"
#include "config_result.hpp"
#include "config_store.hpp"
#include "config_validator.hpp"

#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace nexora::configuration
{

class ConfigManager
{
public:
    ConfigManager() = default;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    [[nodiscard]]
    ConfigResult<ConfigValue> get(const std::string& key) const
    {
        auto result = store_.get(key);

        if (result)
        {
            return result.value();
        }

        auto defaultValue = defaults_.get(key);

        if (defaultValue.has_value())
        {
            return defaultValue.value();
        }

        return result.error();
    }

    [[nodiscard]]
    ConfigResult<bool> set(
        const std::string& key,
        const ConfigValue& value
    )
    {
        if (key.empty())
        {
            return ConfigError{
                ConfigErrorCode::InvalidValue,
                "Configuration key cannot be empty."
            };
        }

        auto validation = validator_.validate(key, value);

        if (!validation)
        {
            return validation.error();
        }

        if (store_.contains(key))
        {
            return store_.set(key, value);
        }

        return store_.insert(key, value);
    }

    [[nodiscard]]
    ConfigResult<bool> setDefault(
        const std::string& key,
        const ConfigValue& value
    )
    {
        if (key.empty())
        {
            return ConfigError{
                ConfigErrorCode::InvalidValue,
                "Default configuration key cannot be empty."
            };
        }

        defaults_.set(key, value);
        return true;
    }

    [[nodiscard]]
    ConfigResult<bool> remove(const std::string& key)
    {
        if (!store_.contains(key))
        {
            return ConfigError{
                ConfigErrorCode::NotFound,
                "Configuration key was not found."
            };
        }

        store_.remove(key);
        return true;
    }

    [[nodiscard]]
    bool contains(const std::string& key) const
    {
        return store_.contains(key) || defaults_.contains(key);
    }

    [[nodiscard]]
    std::vector<std::string> keys() const
    {
        auto result = store_.keys();

        for (const auto& key : defaults_.keys())
        {
            if (std::find(result.begin(), result.end(), key) == result.end())
            {
                result.push_back(key);
            }
        }

        return result;
    }

    [[nodiscard]]
    std::size_t size() const
    {
        return keys().size();
    }

    [[nodiscard]]
    bool empty() const
    {
        return size() == 0;
    }

    void clear()
    {
        store_.clear();
        defaults_.clear();
    }

    [[nodiscard]]
    ConfigStore& store() noexcept
    {
        return store_;
    }

    [[nodiscard]]
    const ConfigStore& store() const noexcept
    {
        return store_;
    }

    [[nodiscard]]
    ConfigDefaults& defaults() noexcept
    {
        return defaults_;
    }

    [[nodiscard]]
    const ConfigDefaults& defaults() const noexcept
    {
        return defaults_;
    }

    [[nodiscard]]
    ConfigValidator& validator() noexcept
    {
        return validator_;
    }

    [[nodiscard]]
    const ConfigValidator& validator() const noexcept
    {
        return validator_;
    }

private:
    ConfigStore store_;
    ConfigDefaults defaults_;
    ConfigValidator validator_;
};

} // namespace nexora::configuration