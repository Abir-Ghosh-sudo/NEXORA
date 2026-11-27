#include "nexora/configuration/environment_config.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace nexora::configuration;

int main()
{
    // Test 1: Set a test environment variable.
#ifdef _WIN32
    _putenv_s("NEXORA_TEST_ENV", "NEXORA");
#else
    setenv("NEXORA_TEST_ENV", "NEXORA", 1);
#endif

    // Test 2: Check whether variable exists.
    assert(EnvironmentConfig::exists("NEXORA_TEST_ENV"));

    // Test 3: Retrieve environment variable.
    auto value = EnvironmentConfig::get("NEXORA_TEST_ENV");

    assert(value.has_value());
    assert(value.value() == "NEXORA");

    // Test 4: Retrieve a missing variable.
    auto missingValue = EnvironmentConfig::get(
        "NEXORA_NON_EXISTENT_ENV"
    );

    assert(!missingValue.has_value());

    // Test 5: Check missing variable.
    assert(!EnvironmentConfig::exists(
        "NEXORA_NON_EXISTENT_ENV"
    ));

    // Test 6: Retrieve variable with default value.
    auto defaultValue = EnvironmentConfig::getOrDefault(
        "NEXORA_NON_EXISTENT_ENV",
        "DEFAULT_VALUE"
    );

    assert(defaultValue.has_value());
    assert(defaultValue.value() == "DEFAULT_VALUE");

    // Test 7: Retrieve existing variable with default.
    auto existingValue = EnvironmentConfig::getOrDefault(
        "NEXORA_TEST_ENV",
        "FALLBACK"
    );

    assert(existingValue.has_value());
    assert(existingValue.value() == "NEXORA");

    // Test 8: Empty variable name.
    auto emptyName = EnvironmentConfig::get("");

    assert(!emptyName.has_value());

    // Cleanup.
#ifdef _WIN32
    _putenv_s("NEXORA_TEST_ENV", "");
#else
    unsetenv("NEXORA_TEST_ENV");
#endif

    std::cout << "[PASS] EnvironmentConfig tests\n";

    return 0;
}