
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

class ConfigSchema
{
public:
    using CustomValidator = std::function<bool(const ConfigValue&)>;

    struct Field
    {
        ConfigType expectedType{ConfigType::Null};
        bool required{false};
        CustomValidator validator{};
        std::string description{};
    };

    ConfigSchema() = default;

    ConfigSchema(const ConfigSchema&) = delete;
    ConfigSchema& operator=(const ConfigSchema&) = delete;

    [[nodiscard]]
    ConfigResult<bool> addField(
        std::string key,
        ConfigType expectedType,
        bool required = false,
        CustomValidator validator = {},
        std::string description = {}
    )
    {
        if (key.empty())
        {
            return ConfigError{
                ConfigErrorCode::InvalidValue,
                "Schema field name cannot be empty."
            };
        }

        if (fields_.find(key) != fields_.end())
        {
            return ConfigError{
                ConfigErrorCode::DuplicateKey,
                "Schema field already exists: " + key
            };
        }

        fields_.emplace(
            std::move(key),
            Field{
                expectedType,
                required,
                std::move(validator),
                std::move(description)
            }
        );

        return true;
    }

    [[nodiscard]]
    bool hasField(const std::string& key) const
    {
        return fields_.find(key) != fields_.end();
    }

    [[nodiscard]]
    const Field* findField(const std::string& key) const noexcept
    {
        const auto iterator = fields_.find(key);

        if (iterator == fields_.end())
        {
            return nullptr;
        }

        return &iterator->second;
    }

    [[nodiscard]]
    ConfigResult<bool> validateField(
        const std::string& key,
        const ConfigValue& value
    ) const
    {
        const auto* field = findField(key);

        if (field == nullptr)
        {
            return ConfigError{
                ConfigErrorCode::NotFound,
                "Schema field not found: " + key
            };
        }

        if (!matchesType(field->expectedType, value))
        {
            return ConfigError{
                ConfigErrorCode::InvalidType,
                "Invalid value type for field: " + key
            };
        }

        if (field->validator && !field->validator(value))
        {
            return ConfigError{
                ConfigErrorCode::ValidationFailed,
                "Custom validation failed for field: " + key
            };
        }

        return true;
    }

    [[nodiscard]]
    ConfigResult<bool> validateRequired(
        const std::vector<std::string>& providedKeys
    ) const
    {
        for (const auto& [key, field] : fields_)
        {
            if (field.required)
            {
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
                        "Required configuration field is missing: " + key
                    };
                }
            }
        }

        return true;
    }

    [[nodiscard]]
    bool removeField(const std::string& key)
    {
        return fields_.erase(key) > 0;
    }

    void clear()
    {
        fields_.clear();
    }

    [[nodiscard]]
    std::size_t fieldCount() const noexcept
    {
        return fields_.size();
    }

    [[nodiscard]]
    bool empty() const noexcept
    {
        return fields_.empty();
    }

    [[nodiscard]]
    const std::unordered_map<std::string, Field>& fields() const noexcept
    {
        return fields_;
    }

private:
    [[nodiscard]]
    static bool matchesType(
        ConfigType expectedType,
        const ConfigValue& value
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
    std::unordered_map<std::string, Field> fields_;
};

} // namespace nexora::configuration
