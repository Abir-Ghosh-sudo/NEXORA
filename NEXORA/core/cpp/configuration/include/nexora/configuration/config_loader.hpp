
#pragma once

#include "config_file.hpp"
#include "config_parser.hpp"

#include <filesystem>
#include <string>

namespace nexora::configuration
{

class ConfigLoader
{
public:
    ConfigLoader() = default;

    [[nodiscard]]
    static ConfigResult<ConfigValue> load(
        const std::filesystem::path& filePath,
        const ConfigParser& parser
    )
    {
        auto fileResult = ConfigFile::read(filePath);

        if (!fileResult)
        {
            return fileResult.error();
        }

        return parser.parse(fileResult.value());
    }

    [[nodiscard]]
    static ConfigResult<bool> loadInto(
        const std::filesystem::path& filePath,
        const ConfigParser& parser,
        ConfigValue& output
    )
    {
        auto result = load(filePath, parser);

        if (!result)
        {
            return result.error();
        }

        output = std::move(result).value();

        return true;
    }
};

} // namespace nexora::configuration
