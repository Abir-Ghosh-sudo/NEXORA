
#include "nexora/configuration/config_error.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

using namespace nexora::configuration;

int main()
{
    static_assert(
        std::is_enum_v<ConfigErrorCode>,
        "ConfigErrorCode must be an enum."
    );

    static_assert(
        std::is_same_v<
            std::underlying_type_t<ConfigErrorCode>,
            std::uint8_t
        >,
        "ConfigErrorCode must use uint8_t."
    );

    // No error
    ConfigError noError;

    assert(!noError.hasError());
    assert(!static_cast<bool>(noError));

    // Invalid value error
    ConfigError invalidError(
        ConfigErrorCode::InvalidValue,
        "Invalid configuration value."
    );

    assert(invalidError.hasError());
    assert(static_cast<bool>(invalidError));

    // Error code
    assert(
        invalidError.code() == ConfigErrorCode::InvalidValue
    );

    // Error message
    assert(
        invalidError.message() == "Invalid configuration value."
    );

    // Different error codes
    ConfigError notFoundError(
        ConfigErrorCode::NotFound,
        "Configuration key not found."
    );

    assert(notFoundError.hasError());
    assert(notFoundError.code() == ConfigErrorCode::NotFound);

    ConfigError parseError(
        ConfigErrorCode::ParseError,
        "Invalid JSON input."
    );

    assert(parseError.hasError());
    assert(parseError.code() == ConfigErrorCode::ParseError);

    // Error message must be a string
    static_assert(
        std::is_same_v<
            decltype(invalidError.message()),
            const std::string&
        >,
        "Error message must return a const string reference."
    );

    std::cout
        << "[PASS] ConfigError tests completed successfully.\n";

    return 0;
}
