
#pragma once

#include "config_error.hpp"
#include "config_result.hpp"
#include "config_value.hpp"

#include <string>
#include <string_view>

namespace nexora::configuration
{

class ConfigParser
{
public:
    virtual ~ConfigParser() = default;

    ConfigParser() = default;

    ConfigParser(const ConfigParser&) = default;
    ConfigParser& operator=(const ConfigParser&) = default;

    ConfigParser(ConfigParser&&) noexcept = default;
    ConfigParser& operator=(ConfigParser&&) noexcept = default;

    [[nodiscard]]
    virtual ConfigResult<ConfigValue> parse(
        std::string_view input
    ) const = 0;

    [[nodiscard]]
    virtual ConfigResult<ConfigValue> parseFile(
        const std::string& filePath
    ) const
    {
        (void)filePath;

        return ConfigError{
            ConfigErrorCode::UnsupportedFormat,
            "File parsing is not supported by this parser."
        };
    }
};

} // namespace nexora::configuration
