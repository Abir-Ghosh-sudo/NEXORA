#include "nexora/configuration/config_parser.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

using namespace nexora::configuration;

// Minimal concrete parser for testing the abstract ConfigParser interface.
class TestParser final : public ConfigParser
{
public:
    ConfigResult<ConfigValue> parse(std::string_view input) const override
    {
        if (input == "NEXORA")
        {
            return ConfigValue("NEXORA");
        }

        if (input == "42")
        {
            return ConfigValue(42);
        }

        return ConfigError{
            ConfigErrorCode::ParseError,
            "Invalid test input"
        };
    }
};

int main()
{
    TestParser parser;

    // Test 1: Parse string successfully.
    auto stringResult = parser.parse("NEXORA");

    assert(stringResult.hasValue());
    assert(stringResult.value().isString());
    assert(*stringResult.value().asString() == "NEXORA");

    // Test 2: Parse integer successfully.
    auto integerResult = parser.parse("42");

    assert(integerResult.hasValue());
    assert(integerResult.value().isInteger());
    assert(*integerResult.value().asInteger() == 42);

    // Test 3: Invalid input should return an error.
    auto invalidResult = parser.parse("INVALID");

    assert(!invalidResult.hasValue());
    assert(invalidResult.error().code() == ConfigErrorCode::ParseError);
    assert(!invalidResult.error().message().empty());

    // Test 4: Empty input should return an error.
    auto emptyResult = parser.parse("");

    assert(!emptyResult.hasValue());
    assert(emptyResult.error().code() == ConfigErrorCode::ParseError);

    // Test 5: Default parseFile implementation.
    auto fileResult = parser.parseFile("test_config.json");

    assert(!fileResult.hasValue());
    assert(fileResult.error().code() == ConfigErrorCode::UnsupportedFormat);

    std::cout << "[PASS] ConfigParser interface tests\n";

    return 0;
}