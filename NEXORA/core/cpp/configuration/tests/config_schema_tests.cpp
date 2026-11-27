
#include "nexora/configuration/config_schema.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace nexora::configuration;

int main()
{
    ConfigSchema schema;

    // Test 1: Initial state.
    assert(schema.empty());
    assert(schema.fieldCount() == 0);

    // Test 2: Add required application name field.
    schema.addField(
        "app.name",
        ConfigType::String,
        true,
        {},
        "Application name"
    );

    assert(schema.hasField("app.name"));

    // Test 3: Add required version field.
    schema.addField(
        "app.version",
        ConfigType::Integer,
        true,
        {},
        "Application version"
    );

    // Test 4: Add optional enabled field.
    schema.addField(
        "app.enabled",
        ConfigType::Boolean,
        false,
        {},
        "Application enabled status"
    );

    assert(schema.fieldCount() == 3);

    // Test 5: Find existing field.
    const auto* foundField = schema.findField("app.name");

    assert(foundField != nullptr);
    assert(foundField->expectedType == ConfigType::String);
    assert(foundField->required);
    assert(foundField->description == "Application name");

    // Test 6: Find missing field.
    const auto* missingField = schema.findField("missing.field");

    assert(missingField == nullptr);

    // Test 7: Validate correct string value.
    auto validName = schema.validateField(
        "app.name",
        ConfigValue("NEXORA")
    );

    assert(validName.hasValue());
    assert(validName.value());

    // Test 8: Validate correct integer value.
    auto validVersion = schema.validateField(
        "app.version",
        ConfigValue(1)
    );

    assert(validVersion.hasValue());
    assert(validVersion.value());

    // Test 9: Validate incorrect type.
    auto invalidName = schema.validateField(
        "app.name",
        ConfigValue(123)
    );

    assert(!invalidName.hasValue());
    assert(
        invalidName.error().code() ==
        ConfigErrorCode::InvalidType
    );

    // Test 10: Validate optional boolean field.
    auto validEnabled = schema.validateField(
        "app.enabled",
        ConfigValue(true)
    );

    assert(validEnabled.hasValue());
    assert(validEnabled.value());

    // Test 11: Validate all required fields.
    std::vector<std::string> availableFields = {
        "app.name",
        "app.version"
    };

    auto requiredResult = schema.validateRequired(availableFields);

    assert(requiredResult.hasValue());
    assert(requiredResult.value());

    // Test 12: Detect missing required field.
    std::vector<std::string> missingRequired = {
        "app.name"
    };

    auto missingResult = schema.validateRequired(missingRequired);

    assert(!missingResult.hasValue());
    assert(
        missingResult.error().code() ==
        ConfigErrorCode::MissingRequiredField
    );

    // Test 13: Remove optional field.
    const bool removed = schema.removeField("app.enabled");

    assert(removed);
    assert(!schema.hasField("app.enabled"));
    assert(schema.fieldCount() == 2);

    // Test 14: Remove a non-existing field.
    const bool removedMissing = schema.removeField("missing.field");

    assert(!removedMissing);
    assert(schema.fieldCount() == 2);

    // Test 15: Clear schema.
    schema.clear();

    assert(schema.empty());
    assert(schema.fieldCount() == 0);

    std::cout << "[PASS] ConfigSchema tests\n";

    return 0;
}
