#include "nexora/configuration/json_config_parser.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace nexora::configuration;

int main()
{
    JsonConfigParser parser;

    // Test 1: Parse null.
    auto nullResult = parser.parse("null");

    assert(nullResult.hasValue());
    assert(nullResult.value().isNull());

    // Test 2: Parse boolean.
    auto boolResult = parser.parse("true");

    assert(boolResult.hasValue());
    assert(boolResult.value().isBoolean());
    assert(*boolResult.value().asBoolean());

    // Test 3: Parse integer.
    auto integerResult = parser.parse("42");

    assert(integerResult.hasValue());
    assert(integerResult.value().isInteger());
    assert(*integerResult.value().asInteger() == 42);

    // Test 4: Parse negative integer.
    auto negativeResult = parser.parse("-25");

    assert(negativeResult.hasValue());
    assert(negativeResult.value().isInteger());
    assert(*negativeResult.value().asInteger() == -25);

    // Test 5: Parse floating-point number.
    auto floatResult = parser.parse("3.14");

    assert(floatResult.hasValue());
    assert(floatResult.value().isFloatingPoint());

    // Test 6: Parse string.
    auto stringResult = parser.parse("\"NEXORA\"");

    assert(stringResult.hasValue());
    assert(stringResult.value().isString());
    assert(*stringResult.value().asString() == "NEXORA");

    // Test 7: Parse string with surrounding whitespace.
    auto whitespaceResult = parser.parse("  \"NEXORA\"  ");

    assert(whitespaceResult.hasValue());
    assert(*whitespaceResult.value().asString() == "NEXORA");

    // Test 8: Invalid JSON input.
    auto invalidResult = parser.parse("invalid");

    assert(!invalidResult.hasValue());
    assert(
        invalidResult.error().code() ==
        ConfigErrorCode::ParseError
    );

    // Test 9: Empty input.
    auto emptyResult = parser.parse("");

    assert(!emptyResult.hasValue());
    assert(
        emptyResult.error().code() ==
        ConfigErrorCode::ParseError
    );

    // Test 10: Unsupported array input in the current parser.
    auto arrayResult = parser.parse("[1, 2, 3]");

    assert(!arrayResult.hasValue());
    assert(
        arrayResult.error().code() ==
        ConfigErrorCode::ParseError
    );

    std::cout << "[PASS] JsonConfigParser tests\n";

    return 0;
}