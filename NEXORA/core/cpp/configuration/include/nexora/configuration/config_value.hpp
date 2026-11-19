
#pragma once

#include "config_type.hpp"

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace nexora::configuration
{

class ConfigValue
{
public:
    using Array = std::vector<ConfigValue>;
    using Object = std::map<std::string, ConfigValue>;

    using Storage = std::variant<
        std::monostate,
        bool,
        std::int64_t,
        double,
        std::string,
        Array,
        Object
    >;

public:
    ConfigValue() = default;

    ConfigValue(std::nullptr_t)
        : value_(std::monostate{})
    {
    }

    ConfigValue(bool value)
        : value_(value)
    {
    }

    ConfigValue(std::int64_t value)
        : value_(value)
    {
    }

    ConfigValue(int value)
        : value_(static_cast<std::int64_t>(value))
    {
    }

    ConfigValue(double value)
        : value_(value)
    {
    }

    ConfigValue(const char* value)
        : value_(std::string(value))
    {
    }

    ConfigValue(std::string value)
        : value_(std::move(value))
    {
    }

    ConfigValue(Array value)
        : value_(std::move(value))
    {
    }

    ConfigValue(Object value)
        : value_(std::move(value))
    {
    }

    [[nodiscard]] ConfigType type() const noexcept
    {
        switch (value_.index())
        {
            case 0: return ConfigType::Null;
            case 1: return ConfigType::Boolean;
            case 2: return ConfigType::Integer;
            case 3: return ConfigType::FloatingPoint;
            case 4: return ConfigType::String;
            case 5: return ConfigType::Array;
            case 6: return ConfigType::Object;
            default: return ConfigType::Null;
        }
    }

    [[nodiscard]] const Storage& storage() const noexcept
    {
        return value_;
    }

    [[nodiscard]] Storage& storage() noexcept
    {
        return value_;
    }

    [[nodiscard]] bool isNull() const noexcept
    {
        return std::holds_alternative<std::monostate>(value_);
    }

    [[nodiscard]] bool isBoolean() const noexcept
    {
        return std::holds_alternative<bool>(value_);
    }

    [[nodiscard]] bool isInteger() const noexcept
    {
        return std::holds_alternative<std::int64_t>(value_);
    }

    [[nodiscard]] bool isFloatingPoint() const noexcept
    {
        return std::holds_alternative<double>(value_);
    }

    [[nodiscard]] bool isString() const noexcept
    {
        return std::holds_alternative<std::string>(value_);
    }

    [[nodiscard]] bool isArray() const noexcept
    {
        return std::holds_alternative<Array>(value_);
    }

    [[nodiscard]] bool isObject() const noexcept
    {
        return std::holds_alternative<Object>(value_);
    }

    [[nodiscard]] const Array* asArray() const noexcept
    {
        return std::get_if<Array>(&value_);
    }

    [[nodiscard]] Array* asArray() noexcept
    {
        return std::get_if<Array>(&value_);
    }

    [[nodiscard]] const Object* asObject() const noexcept
    {
        return std::get_if<Object>(&value_);
    }

    [[nodiscard]] Object* asObject() noexcept
    {
        return std::get_if<Object>(&value_);
    }

    [[nodiscard]] const std::string* asString() const noexcept
    {
        return std::get_if<std::string>(&value_);
    }

    [[nodiscard]] const bool* asBoolean() const noexcept
    {
        return std::get_if<bool>(&value_);
    }

    [[nodiscard]] const std::int64_t* asInteger() const noexcept
    {
        return std::get_if<std::int64_t>(&value_);
    }

    [[nodiscard]] const double* asFloatingPoint() const noexcept
    {
        return std::get_if<double>(&value_);
    }

private:
    Storage value_;
};

} // namespace nexora::configuration
