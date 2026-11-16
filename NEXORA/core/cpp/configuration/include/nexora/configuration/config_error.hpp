
#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace nexora::configuration
{

enum class ConfigErrorCode : std::uint8_t
{
    None = 0,
    NotFound,
    InvalidType,
    InvalidValue,
    ParseError,
    SerializationError,
    FileNotFound,
    FileReadError,
    FileWriteError,
    ValidationFailed,
    DuplicateKey,
    MissingRequiredField,
    EnvironmentVariableError,
    UnsupportedFormat,
    InternalError
};

class ConfigError
{
public:
    ConfigError() = default;

    explicit ConfigError(
        ConfigErrorCode code,
        std::string message = {}
    )
        : code_(code),
          message_(std::move(message))
    {
    }

    [[nodiscard]]
    ConfigErrorCode code() const noexcept
    {
        return code_;
    }

    [[nodiscard]]
    const std::string& message() const noexcept
    {
        return message_;
    }

    [[nodiscard]]
    bool hasError() const noexcept
    {
        return code_ != ConfigErrorCode::None;
    }

    [[nodiscard]]
    explicit operator bool() const noexcept
    {
        return hasError();
    }

private:
    ConfigErrorCode code_{ConfigErrorCode::None};
    std::string message_;
};

} // namespace nexora::configuration
