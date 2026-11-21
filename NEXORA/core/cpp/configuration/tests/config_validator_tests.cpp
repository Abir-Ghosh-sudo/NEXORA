
#include "nexora/configuration/config_validator.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace nexora::configuration;

int main()
{
    ConfigValidator validator;

    // 1. Initial state
    assert(validator.ruleCount() == 0);
    assert(!validator.hasRule("app.name"));

    // 2. Add string rule
    ConfigValidator::Rule nameRule;
    nameRule.expectedType = ConfigType::String;
    nameRule.required = true;
    nameRule.description = "Application name";

    validator.addRule("app.name", nameRule);

    assert(validator.hasRule("app.name"));

    // 3. Add integer rule
    ConfigValidator::Rule versionRule;
    versionRule.expectedType = ConfigType::Integer;
    versionRule.required = true;

    validator.addRule("app.version", versionRule);

    assert(validator.hasRule("app.version"));

    // 4. Add boolean rule
    ConfigValidator::Rule enabledRule;
    enabledRule.expectedType = ConfigType::Boolean;

    validator.addRule("app.enabled", enabledRule);

    assert(validator.hasRule("app.enabled"));

    // 5. Add array rule
    ConfigValidator::Rule featuresRule;
    featuresRule.expectedType = ConfigType::Array;

    validator.addRule("app.features", featuresRule);

    assert(validator.hasRule("app.features"));

    // 6. Add object rule
    ConfigValidator::Rule settingsRule;
    settingsRule.expectedType = ConfigType::Object;

    validator.addRule("app.settings", settingsRule);

    assert(validator.hasRule("app.settings"));

    // 7. Add floating-point rule
    ConfigValidator::Rule ratioRule;
    ratioRule.expectedType = ConfigType::FloatingPoint;

    validator.addRule("app.ratio", ratioRule);

    assert(validator.hasRule("app.ratio"));

    // 8. Add null rule
    ConfigValidator::Rule nullRule;
    nullRule.expectedType = ConfigType::Null;

    validator.addRule("app.optional", nullRule);

    assert(validator.hasRule("app.optional"));
    assert(validator.ruleCount() == 7);

    // 9. Validate string
    auto validName = validator.validate(
        "app.name",
        ConfigValue("NEXORA")
    );

    assert(validName.hasValue());

    // 10. Validate integer
    auto validVersion = validator.validate(
        "app.version",
        ConfigValue(1)
    );

    assert(validVersion.hasValue());

    // 11. Validate boolean
    auto validEnabled = validator.validate(
        "app.enabled",
        ConfigValue(true)
    );

    assert(validEnabled.hasValue());

    // 12. Validate array
    ConfigValue::Array features{
        ConfigValue("AI"),
        ConfigValue("IDE")
    };

    auto validFeatures = validator.validate(
        "app.features",
        ConfigValue(std::move(features))
    );

    assert(validFeatures.hasValue());

    // 13. Validate object
    ConfigValue::Object settings{
        {"theme", ConfigValue("dark")}
    };

    auto validSettings = validator.validate(
        "app.settings",
        ConfigValue(std::move(settings))
    );

    assert(validSettings.hasValue());

    // 14. Validate floating-point
    auto validRatio = validator.validate(
        "app.ratio",
        ConfigValue(1.5)
    );

    assert(validRatio.hasValue());

    // 15. Validate null
    auto validNull = validator.validate(
        "app.optional",
        ConfigValue()
    );

    assert(validNull.hasValue());

    // 16. Reject incorrect type
    auto invalidType = validator.validate(
        "app.version",
        ConfigValue("one")
    );

    assert(!invalidType.hasValue());
    assert(
        invalidType.error().code() ==
        ConfigErrorCode::InvalidType
    );

    // 17. Unknown key
    auto unknownKey = validator.validate(
        "app.unknown",
        ConfigValue(100)
    );

    assert(unknownKey.hasValue());

    // 18. Custom validation for server port
    ConfigValidator::Rule portRule;
    portRule.expectedType = ConfigType::Integer;

    portRule.customValidator = [](const ConfigValue& value)
    {
        const auto* port = value.asInteger();

        return port != nullptr &&
               *port > 0 &&
               *port <= 65535;
    };

    validator.addRule("server.port", portRule);

    assert(validator.hasRule("server.port"));

    // Valid port
    auto validPort = validator.validate(
        "server.port",
        ConfigValue(8080)
    );

    assert(validPort.hasValue());

    // Invalid port
    auto invalidPort = validator.validate(
        "server.port",
        ConfigValue(70000)
    );

    assert(!invalidPort.hasValue());
    assert(
        invalidPort.error().code() ==
        ConfigErrorCode::ValidationFailed
    );

    // 19. Validate required fields
    std::vector<std::string> availableKeys{
        "app.name",
        "app.version"
    };

    auto validRequired = validator.validateRequired(
        availableKeys
    );

    assert(validRequired.hasValue());

    // Missing required field
    std::vector<std::string> missingKeys{
        "app.version"
    };

    auto invalidRequired = validator.validateRequired(
        missingKeys
    );

    assert(!invalidRequired.hasValue());
    assert(
        invalidRequired.error().code() ==
        ConfigErrorCode::MissingRequiredField
    );

    // 20. Remove existing rule
    const bool removed = validator.removeRule("app.enabled");

    assert(removed);
    assert(!validator.hasRule("app.enabled"));

    // 21. Remove missing rule
    const bool missingRemoval =
        validator.removeRule("app.missing");

    assert(!missingRemoval);

    // 22. Clear all rules
    validator.clearRules();

    assert(validator.ruleCount() == 0);
    assert(!validator.hasRule("app.name"));

    std::cout
        << "[PASS] ConfigValidator tests completed successfully.\n";

    return 0;
}