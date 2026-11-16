
#pragma once

#include "config_serializer.hpp"

#include <charconv>
#include <string>
#include <system_error>

namespace nexora::configuration
{

class JsonConfigSerializer final : public ConfigSerializer
{
public:
    [[nodiscard]]
    ConfigResult<std::string> serialize(
        const ConfigValue& value
    ) const override
    {
        const auto& storage = value.storage();

        if (std::holds_alternative<std::monostate>(storage))
        {
            return std::string{"null"};
        }

        if (const auto* boolean = std::get_if<bool>(&storage))
        {
            return std::string{*boolean ? "true" : "false"};
        }

        if (const auto* integer = std::get_if<std::int64_t>(&storage))
        {
            return std::to_string(*integer);
        }

        if (const auto* floating = std::get_if<double>(&storage))
        {
            if (!std::isfinite(*floating))
            {
                return ConfigError{
                    ConfigErrorCode::SerializationError,
                    "Non-finite floating-point values are not valid JSON."
                };
            }

            char buffer[128];

            const auto result = std::to_chars(
                buffer,
                buffer + sizeof(buffer),
                *floating
            );

            if (result.ec != std::errc{})
            {
                return ConfigError{
                    ConfigErrorCode::SerializationError,
                    "Failed to serialize floating-point value."
                };
            }

            return std::string(buffer, result.ptr);
        }

        if (const auto* string = std::get_if<std::string>(&storage))
        {
            return serializeString(*string);
        }

        return ConfigError{
            ConfigErrorCode::SerializationError,
            "Unsupported configuration value."
        };
    }

private:
    [[nodiscard]]
    static std::string serializeString(const std::string& value)
    {
        std::string result;
        result.reserve(value.size() + 2);

        result.push_back('"');

        constexpr char hexDigits[] = "0123456789abcdef";

        for (const unsigned char ch : value)
        {
            switch (ch)
            {
                case '"':
                    result += "\\\"";
                    break;

                case '\\':
                    result += "\\\\";
                    break;

                case '\b':
                    result += "\\b";
                    break;

                case '\f':
                    result += "\\f";
                    break;

                case '\n':
                    result += "\\n";
                    break;

                case '\r':
                    result += "\\r";
                    break;

                case '\t':
                    result += "\\t";
                    break;

                default:
                    if (ch < 0x20)
                    {
                        result += "\\u00";
                        result.push_back(hexDigits[(ch >> 4) & 0x0F]);
                        result.push_back(hexDigits[ch & 0x0F]);
                    }
                    else
                    {
                        result.push_back(static_cast<char>(ch));
                    }

                    break;
            }
        }

        result.push_back('"');

        return result;
    }
};

} // namespace nexora::configuration
