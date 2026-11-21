
#include "nexora/configuration/config_validator.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace nexora::configuration;

int main()
{
    ConfigValidator validator;

    // 1. Initial state
    assert(validator.ruleCount() == 0);

    // 2. Add string rule
    ConfigValidator::Rule nameRule;
    nameRule.expectedType = ConfigType::String;
    nameRule.required = true;
    nameRule.description = "Application name";

    auto addNameRule = validator.addRule(
        "app.name",
        nameRule
    );

    assert(addNameRule.hasValue());
    assert(addNameRule.value());
    assert(validator.hasRule("app.name"));

    // 3. Add integer rule
    ConfigValidator::Rule versionRule;
    versionRule.expectedType = ConfigType::Integer;
    versionRule.required = true;

    assert(
        validator.addRule("app.version", versionRule).hasValue()
    );

    // 4. Add boolean rule
    ConfigValidator::Rule enabledRule;
    enabledRule.expectedType = ConfigType::Boolean;

    assert(
        validator.addRule("app.enabled", enabledRule).hasValue()
    );

    // 5. Add array rule
    ConfigValidator::Rule featuresRule;
    featuresRule.expectedType = ConfigType::Array;

    assert(
        validator.addRule("app.features", featuresRule).hasValue()
    );

    // 6. Add object rule
    ConfigValidator::Rule settingsRule;
    settingsRule.expectedType = ConfigType::Object;

    assert(
        validator.addRule("app.settings", settingsRule).hasValue()
    );

    // 7. Add floating-point rule
    ConfigValidator::Rule ratioRule;
    ratioRule.expectedType = ConfigType::FloatingPoint;

    assert(
        validator.addRule("app.ratio", ratioRule).hasValue()
    );

    // 8. Add null rule
    ConfigValidator::Rule nullRule;
    nullRule.expectedType = ConfigType::Null;

    assert(
        validator.addRule("app.optional", nullRule).hasValue()
    );

    assert(validator.ruleCount() == 7);

    // 9. Validate correct types
    assert(validator.validate(
        "app.name", ConfigValue("NEXORA")
    ).hasValue());

    assert(validator.validate(
        "app.version", ConfigValue(1)
    ).hasValue());

    assert(validator.validate(
        "app.enabled", ConfigValue(true)
    ).hasValue());

    assert(validator.validate(
        "app.features",
        ConfigValue::Array{
            ConfigValue("AI"),
            ConfigValue("IDE")
        }
    ).hasValue());

    assert(validator.validate(
        "app.settings",
        ConfigValue::Object{
            {"theme", ConfigValue("dark")}
        }
    ).hasValue());

    assert(validator.validate(
        "app.ratio", ConfigValue(1.5)
    ).hasValue());

    assert(validator.validate(
        "app.optional", ConfigValue()
    ).hasValue());

    // 10. Reject incorrect type
    auto invalidType = validator.validate(
        "app.version",
        ConfigValue("one")
    );

    assert(!invalidType.hasValue());
    assert(
        invalidType.error().code() == ConfigErrorCode::InvalidType
    );

    // 11. Unknown key has no validation rule
    assert(validator.validate(
        "app.unknown", ConfigValue(100)
    ).hasValue());

    // 12. Custom validation
    ConfigValidator::Rule portRule;
    portRule.expectedType = ConfigType::Integer;
    portRule.customValidator = [](const ConfigValue& value)
    {
        const auto* port = value.asInteger();
        return port != nullptr && *port > 0 && *port <= 65535;
    };

    assert(
        validator.addRule("server.port", portRule).hasValue()
    );

    assert(validator.validate(
        "server.port", ConfigValue(8080)
    ).hasValue());

    auto invalidPort = validator.validate(
        "server.port", ConfigValue(70000)
    );

    assert(!invalidPort.hasValue());
    assert(
        invalidPort.error().code() ==
        ConfigErrorCode::ValidationFailed
    );

    // 13. Required fields
    std::vector<std::string> availableKeys{
        "app.name",
        "app.version"
    };

    assert(validator.validateRequired(availableKeys).hasValue());

    std::vector<std::string> missingKeys{
        "app.version"
    };

    auto requiredResult = validator.validateRequired(missingKeys);

    assert(!requiredResult.hasValue());
    assert(
        requiredResult.error().code() ==
        ConfigErrorCode::MissingRequiredField
    );

    // 14. Remove rule
    assert(validator.removeRule("app.enabled"));
    assert(!validator.hasRule("app.enabled"));

    // 15. Remove missing rule
    assert(!validator.removeRule("app.missing"));

    // 16. Clear rules
    validator.clearRules();

    assert(validator.ruleCount() == 0);

    std::cout
        << "[PASS] ConfigValidator tests completed successfully.\n";

    return 0;
}