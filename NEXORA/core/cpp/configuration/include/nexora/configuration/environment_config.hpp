
#pragma once

#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>

namespace nexora::configuration
{

class EnvironmentConfig
{
public:
    EnvironmentConfig() = default;

    [[nodiscard]]
    static std::optional<std::string> get(
        std::string_view key
    )
    {
        if (key.empty())
        {
            return std::nullopt;
        }

        const std::string keyString{key};

        const char* value = std::getenv(keyString.c_str());

        if (value == nullptr)
        {
            return std::nullopt;
        }

        return std::string{value};
    }

    [[nodiscard]]
    static bool exists(std::string_view key)
    {
        return get(key).has_value();
    }

    [[nodiscard]]
    static std::optional<std::string> getOrDefault(
        std::string_view key,
        std::string defaultValue
    )
    {
        auto value = get(key);

        if (value.has_value())
        {
            return value;
        }

        return std::move(defaultValue);
    }
};

} // namespace nexora::configuration
