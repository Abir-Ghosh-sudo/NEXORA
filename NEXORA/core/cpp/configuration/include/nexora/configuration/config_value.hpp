
#pragma once

#include <cstdint>
#include <string>
#include <variant>

namespace nexora::configuration
{

class ConfigValue
{
public:
    using Storage = std::variant<
        std::monostate,
        bool,
        std::int64_t,
        double,
        std::string
    >;

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

    [[nodiscard]]
    const Storage& storage() const noexcept
    {
        return value_;
    }

    [[nodiscard]]
    bool isNull() const noexcept
    {
        return std::holds_alternative<std::monostate>(value_);
    }

    [[nodiscard]]
    bool isBoolean() const noexcept
    {
        return std::holds_alternative<bool>(value_);
    }

    [[nodiscard]]
    bool isInteger() const noexcept
    {
        return std::holds_alternative<std::int64_t>(value_);
    }

    [[nodiscard]]
    bool isFloatingPoint() const noexcept
    {
        return std::holds_alternative<double>(value_);
    }

    [[nodiscard]]
    bool isString() const noexcept
    {
        return std::holds_alternative<std::string>(value_);
    }

private:
    Storage value_{};
};

} // namespace nexora::configuration
