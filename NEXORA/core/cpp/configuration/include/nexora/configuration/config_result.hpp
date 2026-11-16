
#pragma once

#include "config_error.hpp"

#include <expected>
#include <utility>

namespace nexora::configuration
{

template <typename T>
class ConfigResult
{
public:
    using ValueType = T;
    using ErrorType = ConfigError;

    ConfigResult(const T& value)
        : result_(value)
    {
    }

    ConfigResult(T&& value)
        : result_(std::move(value))
    {
    }

    ConfigResult(const ConfigError& error)
        : result_(std::unexpected(error))
    {
    }

    ConfigResult(ConfigError&& error)
        : result_(std::unexpected(std::move(error)))
    {
    }

    [[nodiscard]]
    bool hasValue() const noexcept
    {
        return result_.has_value();
    }

    [[nodiscard]]
    explicit operator bool() const noexcept
    {
        return hasValue();
    }

    [[nodiscard]]
    T& value() &
    {
        return result_.value();
    }

    [[nodiscard]]
    const T& value() const&
    {
        return result_.value();
    }

    [[nodiscard]]
    T&& value() &&
    {
        return std::move(result_).value();
    }

    [[nodiscard]]
    const ConfigError& error() const&
    {
        return result_.error();
    }

    [[nodiscard]]
    ConfigError&& error() &&
    {
        return std::move(result_).error();
    }

private:
    std::expected<T, ConfigError> result_;
};

} // namespace nexora::configuration
