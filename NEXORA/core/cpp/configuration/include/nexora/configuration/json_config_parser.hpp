
#pragma once

#include "config_parser.hpp"

#include <charconv>
#include <cctype>
#include <string>
#include <string_view>
#include <system_error>

namespace nexora::configuration
{

class JsonConfigParser final : public ConfigParser
{
public:
    [[nodiscard]]
    ConfigResult<ConfigValue> parse(
        std::string_view input
    ) const override
    {
        input = trim(input);

        if (input.empty())
        {
            return parseError("JSON input is empty.");
        }

        if (input == "null")
        {
            return ConfigValue{};
        }

        if (input == "true")
        {
            return ConfigValue{true};
        }

        if (input == "false")
        {
            return ConfigValue{false};
        }

        if (input.front() == '"')
        {
            return parseString(input);
        }

        if (input.front() == '{' || input.front() == '[')
        {
            return parseError(
                "JSON objects and arrays are not supported yet."
            );
        }

        return parseNumber(input);
    }

private:
    [[nodiscard]]
    static std::string_view trim(
        std::string_view input
    ) noexcept
    {
        while (!input.empty() &&
               std::isspace(static_cast<unsigned char>(input.front())))
        {
            input.remove_prefix(1);
        }

        while (!input.empty() &&
               std::isspace(static_cast<unsigned char>(input.back())))
        {
            input.remove_suffix(1);
        }

        return input;
    }

    [[nodiscard]]
    static ConfigResult<ConfigValue> parseError(
        std::string message
    )
    {
        return ConfigError{
            ConfigErrorCode::ParseError,
            std::move(message)
        };
    }

    [[nodiscard]]
    static ConfigResult<ConfigValue> parseString(
        std::string_view input
    )
    {
        if (input.size() < 2 ||
            input.back() != '"')
        {
            return parseError("Invalid JSON string.");
        }

        std::string result;

        for (std::size_t i = 1; i < input.size() - 1; ++i)
        {
            const char ch = input[i];

            if (ch == '\\')
            {
                return parseError(
                    "JSON escape sequences are not supported yet."
                );
            }

            if (static_cast<unsigned char>(ch) < 0x20)
            {
                return parseError(
                    "Unescaped control character in JSON string."
                );
            }

            result.push_back(ch);
        }

        return ConfigValue{std::move(result)};
    }

    [[nodiscard]]
    static ConfigResult<ConfigValue> parseNumber(
        std::string_view input
    )
    {
        std::int64_t integerValue{};

        const auto integerResult = std::from_chars(
            input.data(),
            input.data() + input.size(),
            integerValue
        );

        if (integerResult.ec == std::errc{} &&
            integerResult.ptr == input.data() + input.size())
        {
            return ConfigValue{integerValue};
        }

        double floatingValue{};

        const auto floatingResult = std::from_chars(
            input.data(),
            input.data() + input.size(),
            floatingValue
        );

        if (floatingResult.ec == std::errc{} &&
            floatingResult.ptr == input.data() + input.size())
        {
            return ConfigValue{floatingValue};
        }

        return parseError("Invalid JSON value.");
    }
};

} // namespace nexora::configuration
