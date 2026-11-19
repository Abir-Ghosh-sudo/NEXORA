
#include "nexora/configuration/config_value.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

using namespace nexora::configuration;

int main()
{
    // Null
    ConfigValue nullValue;
    assert(nullValue.isNull());
    assert(nullValue.type() == ConfigType::Null);

    // Boolean
    ConfigValue boolValue(true);
    assert(boolValue.isBoolean());
    assert(boolValue.type() == ConfigType::Boolean);
    assert(*boolValue.asBoolean());

    // Integer
    ConfigValue intValue(std::int64_t{42});
    assert(intValue.isInteger());
    assert(intValue.type() == ConfigType::Integer);
    assert(*intValue.asInteger() == 42);

    // Floating Point
    ConfigValue floatValue(3.14);
    assert(floatValue.isFloatingPoint());
    assert(floatValue.type() == ConfigType::FloatingPoint);

    // String
    ConfigValue stringValue(std::string("NEXORA"));
    assert(stringValue.isString());
    assert(stringValue.type() == ConfigType::String);
    assert(*stringValue.asString() == "NEXORA");

    // Array
    ConfigValue::Array array{
        ConfigValue(10),
        ConfigValue("AI"),
        ConfigValue(true)
    };

    ConfigValue arrayValue(std::move(array));

    assert(arrayValue.isArray());
    assert(arrayValue.type() == ConfigType::Array);
    assert(arrayValue.asArray()->size() == 3);

    // Object
    ConfigValue::Object object{
        {"name", ConfigValue("NEXORA")},
        {"version", ConfigValue(1)},
        {"enabled", ConfigValue(true)}
    };

    ConfigValue objectValue(std::move(object));

    assert(objectValue.isObject());
    assert(objectValue.type() == ConfigType::Object);
    assert(objectValue.asObject()->size() == 3);

    // Nested Object and Array
    ConfigValue nestedValue(
        ConfigValue::Object{
            {
                "project",
                ConfigValue("NEXORA")
            },
            {
                "features",
                ConfigValue::Array{
                    ConfigValue("AI"),
                    ConfigValue("IDE"),
                    ConfigValue("Runtime")
                }
            }
        }
    );

    assert(nestedValue.isObject());

    const auto* nestedObject = nestedValue.asObject();
    assert(nestedObject != nullptr);

    assert(nestedObject->at("project").isString());
    assert(nestedObject->at("features").isArray());

    assert(
        nestedObject->at("features").asArray()->size() == 3
    );

    std::cout
        << "[PASS] ConfigValue tests completed successfully.\n";

    return 0;
}
