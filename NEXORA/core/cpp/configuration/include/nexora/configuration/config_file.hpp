
#pragma once

#include "config_error.hpp"
#include "config_result.hpp"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>
#include <utility>

namespace nexora::configuration
{

class ConfigFile
{
public:
    ConfigFile() = default;

    [[nodiscard]]
    static ConfigResult<std::string> read(
        const std::filesystem::path& filePath
    )
    {
        std::error_code error;

        if (!std::filesystem::exists(filePath, error))
        {
            return ConfigError{
                ConfigErrorCode::FileNotFound,
                "Configuration file does not exist: " +
                    filePath.string()
            };
        }

        if (error)
        {
            return ConfigError{
                ConfigErrorCode::FileReadError,
                "Failed to inspect configuration file: " +
                    error.message()
            };
        }

        std::ifstream file(filePath, std::ios::binary);

        if (!file.is_open())
        {
            return ConfigError{
                ConfigErrorCode::FileReadError,
                "Unable to open configuration file: " +
                    filePath.string()
            };
        }

        std::string content{
            std::istreambuf_iterator<char>{file},
            std::istreambuf_iterator<char>{}
        };

        if (file.bad())
        {
            return ConfigError{
                ConfigErrorCode::FileReadError,
                "Failed while reading configuration file: " +
                    filePath.string()
            };
        }

        return content;
    }

    [[nodiscard]]
    static ConfigResult<bool> write(
        const std::filesystem::path& filePath,
        const std::string& content
    )
    {
        if (filePath.empty())
        {
            return ConfigError{
                ConfigErrorCode::FileWriteError,
                "Configuration file path cannot be empty."
            };
        }

        std::ofstream file(
            filePath,
            std::ios::binary | std::ios::trunc
        );

        if (!file.is_open())
        {
            return ConfigError{
                ConfigErrorCode::FileWriteError,
                "Unable to open configuration file for writing: " +
                    filePath.string()
            };
        }

        file.write(
            content.data(),
            static_cast<std::streamsize>(content.size())
        );

        file.flush();

        if (!file.good())
        {
            return ConfigError{
                ConfigErrorCode::FileWriteError,
                "Failed to write configuration file: " +
                    filePath.string()
            };
        }

        return true;
    }

    [[nodiscard]]
    static bool exists(
        const std::filesystem::path& filePath
    )
    {
        std::error_code error;

        const bool result =
            std::filesystem::exists(filePath, error);

        return !error && result;
    }

    [[nodiscard]]
    static ConfigResult<bool> remove(
        const std::filesystem::path& filePath
    )
    {
        std::error_code error;

        const bool removed =
            std::filesystem::remove(filePath, error);

        if (error)
        {
            return ConfigError{
                ConfigErrorCode::FileWriteError,
                "Failed to remove configuration file: " +
                    error.message()
            };
        }

        if (!removed)
        {
            return ConfigError{
                ConfigErrorCode::FileNotFound,
                "Configuration file was not found: " +
                    filePath.string()
            };
        }

        return true;
    }
};

} // namespace nexora::configuration