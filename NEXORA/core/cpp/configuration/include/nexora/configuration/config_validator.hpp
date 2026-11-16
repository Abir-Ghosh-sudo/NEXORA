
#pragma once

#include "config_error.hpp"
#include "config_result.hpp"
#include "config_type.hpp"
#include "config_value.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace nexora::configuration
{

class ConfigValidator
{
public:
    using ValidationRule =
        std::function<bool(const ConfigValue&)>;

    struct Rule
    {
        ConfigType expectedType{ConfigType::Null};
        bool required{false};
        ValidationRule customValidator{};
        std::string description{};
    };

    ConfigValidator() = default;

    void addRule(
        std::string key,
        Rule rule
    )
    {
        rules_.insert_or_assign(
            std::move(key),
            std::move(rule)
        );
    }

    [[nodiscard]]
    bool hasRule(const std::string& key) const
    {
        return rules_.find(key) != rules_.end();
    }

    [[nodiscard]]
    ConfigResult<bool> validate(
        const std::string& key,
        const ConfigValue& value
    ) const
    {
        const auto it = rules_.find(key);

        if (it == rules_.end())
        {
            return true;
        }

        const Rule& rule = it->second;

        if (!matchesType(value, rule.expectedType))
        {
            return ConfigError{
                ConfigErrorCode::InvalidType,
                "Invalid configuration type for key: " + key
            };
        }

        if (rule.customValidator &&
            !rule.customValidator(value))
        {
            return ConfigError{
                ConfigErrorCode::ValidationFailed,
                "Configuration validation failed for key: " + key
            };
        }

        return true;
    }

    [[nodiscard]]
    ConfigResult<bool> validateRequired(
        const std::vector<std::string>& providedKeys
    ) const
    {
        for (const auto& [key, rule] : rules_)
        {
            if (!rule.required)
            {
                continue;
            }

            bool found = false;

            for (const auto& providedKey : providedKeys)
            {
                if (providedKey == key)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                return ConfigError{
                    ConfigErrorCode::MissingRequiredField,
                    "Required configuration key is missing: " + key
                };
            }
        }

        return true;
    }

    [[nodiscard]]
    bool removeRule(const std::string& key)
    {
        return rules_.erase(key) > 0;
    }

    void clearRules()
    {
        rules_.clear();
    }

    [[nodiscard]]
    std::size_t ruleCount() const noexcept
    {
        return rules_.size();
    }

private:
    [[nodiscard]]
    static bool matchesType(
        const ConfigValue& value,
        ConfigType expectedType
    ) noexcept
    {
        switch (expectedType)
        {
            case ConfigType::Null:
                return value.isNull();

            case ConfigType::Boolean:
                return value.isBoolean();

            case ConfigType::Integer:
                return value.isInteger();

            case ConfigType::FloatingPoint:
                return value.isFloatingPoint();

            case ConfigType::String:
                return value.isString();

            case ConfigType::Array:
            case ConfigType::Object:
                return false;
        }

        return false;
    }

private:
    std::unordered_map<std::string, Rule> rules_;
};

} // namespace nexora::configuration
