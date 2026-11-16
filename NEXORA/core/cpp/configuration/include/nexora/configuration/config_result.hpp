
#pragma once

#include "config_error.hpp"

#include <utility>
#include <variant>

namespace nexora::configuration
{

template <typename T>
class ConfigResult
{
public:
    using ValueType = T;
    using ErrorType = ConfigError;

    ConfigResult(const T& value)
        : result_(std::in_place_index<0>, value)
    {
    }

    ConfigResult(T&& value)
        : result_(std::in_place_index<0>, std::move(value))
    {
    }

    ConfigResult(const ConfigError& error)
        : result_(std::in_place_index<1>, error)
    {
    }

    ConfigResult(ConfigError&& error)
        : result_(std::in_place_index<1>, std::move(error))
    {
    }

    [[nodiscard]]
    bool hasValue() const noexcept
    {
        return result_.index() == 0;
    }

    [[nodiscard]]
    explicit operator bool() const noexcept
    {
        return hasValue();
    }

    [[nodiscard]]
    T& value() &
    {
        return std::get<0>(result_);
    }

    [[nodiscard]]
    const T& value() const&
    {
        return std::get<0>(result_);
    }

    [[nodiscard]]
    T&& value() &&
    {
        return std::move(std::get<0>(result_));
    }

    [[nodiscard]]
    ConfigError& error() &
    {
        return std::get<1>(result_);
    }

    [[nodiscard]]
    const ConfigError& error() const&
    {
        return std::get<1>(result_);
    }

    [[nodiscard]]
    ConfigError&& error() &&
    {
        return std::move(std::get<1>(result_));
    }

private:
    std::variant<T, ConfigError> result_;
};

} // namespace nexora::configuration
