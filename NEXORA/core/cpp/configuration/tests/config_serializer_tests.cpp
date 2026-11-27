#include "nexora/configuration/config_serializer.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace nexora::configuration;

// Concrete serializer for testing the abstract interface.
class TestSerializer final : public ConfigSerializer
{
public:
    ConfigResult<std::string> serialize(
        const ConfigValue& value) const override
    {
        if (value.isString())
        {
            return *value.asString();
        }

        if (value.isInteger())
        {
            return std::to_string(*value.asInteger());
        }

        return ConfigError{
            ConfigErrorCode::SerializationError,
            "Unsupported test value"
        };
    }
};

int main()
{
    TestSerializer serializer;

    // Test 1: Serialize string.
    ConfigValue stringValue("NEXORA");

    auto stringResult = serializer.serialize(stringValue);

    assert(stringResult.hasValue());
    assert(stringResult.value() == "NEXORA");

    // Test 2: Serialize integer.
    ConfigValue integerValue(42);

    auto integerResult = serializer.serialize(integerValue);

    assert(integerResult.hasValue());
    assert(integerResult.value() == "42");

    // Test 3: Serialize empty string.
    ConfigValue emptyString("");

    auto emptyResult = serializer.serialize(emptyString);

    assert(emptyResult.hasValue());
    assert(emptyResult.value().empty());

    // Test 4: Unsupported boolean value.
    ConfigValue booleanValue(true);

    auto booleanResult = serializer.serialize(booleanValue);

    assert(!booleanResult.hasValue());
    assert(
        booleanResult.error().code() ==
        ConfigErrorCode::SerializationError
    );

    // Test 5: Unsupported null value.
    ConfigValue nullValue;

    auto nullResult = serializer.serialize(nullValue);

    assert(!nullResult.hasValue());
    assert(
        nullResult.error().code() ==
        ConfigErrorCode::SerializationError
    );

    std::cout << "[PASS] ConfigSerializer interface tests\n";

    return 0;
}