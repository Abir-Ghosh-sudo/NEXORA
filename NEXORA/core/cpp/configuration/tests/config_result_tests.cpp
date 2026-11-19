
#include "nexora/configuration/config_result.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

using namespace nexora::configuration;

int main()
{
    // Successful result with integer
    ConfigResult<int> successResult(42);

    assert(successResult.hasValue());
    assert(static_cast<bool>(successResult));
    assert(successResult.value() == 42);

    // Successful result with string
    ConfigResult<std::string> stringResult(
        std::string("NEXORA")
    );

    assert(stringResult.hasValue());
    assert(stringResult.value() == "NEXORA");

    // Error result
    ConfigResult<int> errorResult(
        ConfigError{
            ConfigErrorCode::InvalidValue,
            "Invalid value."
        }
    );

    assert(!errorResult.hasValue());
    assert(!static_cast<bool>(errorResult));
    assert(errorResult.error().hasError());
    assert(errorResult.error().code() == ConfigErrorCode::InvalidValue);

    // Error message
    assert(errorResult.error().message() == "Invalid value.");

    // Move construction
    ConfigResult<std::string> movableResult(
        std::string("Configuration")
    );

    std::string movedValue = std::move(movableResult).value();

    assert(movedValue == "Configuration");

    // Const value access
    const ConfigResult<int> constResult(100);

    assert(constResult.hasValue());
    assert(constResult.value() == 100);

    // Type aliases
    static_assert(
        std::is_same_v<
            ConfigResult<int>::ValueType,
            int
        >
    );

    static_assert(
        std::is_same_v<
            ConfigResult<int>::ErrorType,
            ConfigError
        >
    );

    std::cout
        << "[PASS] ConfigResult tests completed successfully.\n";

    return 0;
}
