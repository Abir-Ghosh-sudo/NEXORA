#include "nexora/configuration/json_config_serializer.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>

using namespace nexora::configuration;

int main()
{
    JsonConfigSerializer serializer;

    // Test 1: Serialize null.
    ConfigValue nullValue;

    auto nullResult = serializer.serialize(nullValue);

    assert(nullResult.hasValue());
    assert(nullResult.value() == "null");

    // Test 2: Serialize boolean true.
    ConfigValue trueValue(true);

    auto trueResult = serializer.serialize(trueValue);

    assert(trueResult.hasValue());
    assert(trueResult.value() == "true");

    // Test 3: Serialize boolean false.
    ConfigValue falseValue(false);

    auto falseResult = serializer.serialize(falseValue);

    assert(falseResult.hasValue());
    assert(falseResult.value() == "false");

    // Test 4: Serialize integer.
    ConfigValue integerValue(42);

    auto integerResult = serializer.serialize(integerValue);

    assert(integerResult.hasValue());
    assert(integerResult.value() == "42");

    // Test 5: Serialize negative integer.
    ConfigValue negativeValue(-25);

    auto negativeResult = serializer.serialize(negativeValue);

    assert(negativeResult.hasValue());
    assert(negativeResult.value() == "-25");

    // Test 6: Serialize floating-point value.
    ConfigValue floatValue(3.14);

    auto floatResult = serializer.serialize(floatValue);

    assert(floatResult.hasValue());
    assert(!floatResult.value().empty());

    // Test 7: Serialize string.
    ConfigValue stringValue("NEXORA");

    auto stringResult = serializer.serialize(stringValue);

    assert(stringResult.hasValue());
    assert(stringResult.value() == "\"NEXORA\"");

    // Test 8: Serialize string containing quotes.
    ConfigValue quoteValue("Hello \"NEXORA\"");

    auto quoteResult = serializer.serialize(quoteValue);

    assert(quoteResult.hasValue());
    assert(quoteResult.value() == "\"Hello \\\"NEXORA\\\"\"");

    // Test 9: Serialize string containing newline.
    ConfigValue newlineValue("Hello\nWorld");

    auto newlineResult = serializer.serialize(newlineValue);

    assert(newlineResult.hasValue());
    assert(newlineResult.value() == "\"Hello\\nWorld\"");

    // Test 10: Reject non-finite floating-point value.
    ConfigValue infiniteValue(
        std::numeric_limits<double>::infinity()
    );

    auto infiniteResult = serializer.serialize(infiniteValue);

    assert(!infiniteResult.hasValue());
    assert(
        infiniteResult.error().code() ==
        ConfigErrorCode::SerializationError
    );

    // Test 11: Reject NaN.
    ConfigValue nanValue(
        std::numeric_limits<double>::quiet_NaN()
    );

    auto nanResult = serializer.serialize(nanValue);

    assert(!nanResult.hasValue());
    assert(
        nanResult.error().code() ==
        ConfigErrorCode::SerializationError
    );

    std::cout << "[PASS] JsonConfigSerializer tests\n";

    return 0;
}