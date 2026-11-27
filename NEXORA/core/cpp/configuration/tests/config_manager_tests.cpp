
#include "nexora/configuration/config_manager.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace nexora::configuration;

int main()
{
    ConfigManager manager;

    // Test 1: Initial state.
    assert(manager.empty());
    assert(manager.size() == 0);

    // Test 2: Set configuration values.
    auto nameResult = manager.set(
        "app.name",
        ConfigValue("NEXORA")
    );

    assert(nameResult.hasValue());
    assert(nameResult.value());

    auto versionResult = manager.set(
        "app.version",
        ConfigValue(1)
    );

    assert(versionResult.hasValue());
    assert(versionResult.value());

    auto enabledResult = manager.set(
        "app.enabled",
        ConfigValue(true)
    );

    assert(enabledResult.hasValue());
    assert(enabledResult.value());

    // Test 3: Retrieve configuration values.
    auto name = manager.get("app.name");

    assert(name.hasValue());
    assert(name.value().isString());
    assert(*name.value().asString() == "NEXORA");

    auto version = manager.get("app.version");

    assert(version.hasValue());
    assert(version.value().isInteger());
    assert(*version.value().asInteger() == 1);

    auto enabled = manager.get("app.enabled");

    assert(enabled.hasValue());
    assert(enabled.value().isBoolean());
    assert(*enabled.value().asBoolean());

    // Test 4: Check existence.
    assert(manager.contains("app.name"));
    assert(manager.contains("app.version"));
    assert(manager.contains("app.enabled"));
    assert(!manager.contains("app.missing"));

    // Test 5: Update existing configuration.
    auto updateResult = manager.set(
        "app.version",
        ConfigValue(2)
    );

    assert(updateResult.hasValue());
    assert(updateResult.value());

    auto updatedVersion = manager.get("app.version");

    assert(updatedVersion.hasValue());
    assert(*updatedVersion.value().asInteger() == 2);

    // Test 6: Default configuration.
    auto defaultResult = manager.setDefault(
        "app.theme",
        ConfigValue("dark")
    );

    assert(defaultResult.hasValue());
    assert(defaultResult.value());

    assert(manager.contains("app.theme"));

    auto theme = manager.get("app.theme");

    assert(theme.hasValue());
    assert(theme.value().isString());
    assert(*theme.value().asString() == "dark");

    // Test 7: Stored value should take priority over default.
    auto overrideDefault = manager.set(
        "app.theme",
        ConfigValue("light")
    );

    assert(overrideDefault.hasValue());
    assert(overrideDefault.value());

    auto currentTheme = manager.get("app.theme");

    assert(currentTheme.hasValue());
    assert(*currentTheme.value().asString() == "light");

    // Test 8: Missing configuration.
    auto missing = manager.get("missing.key");

    assert(!missing.hasValue());
    assert(missing.error().code() == ConfigErrorCode::NotFound);

    // Test 9: Keys and size.
    auto keys = manager.keys();

    assert(keys.size() == 4);
    assert(manager.size() == 4);
    assert(!manager.empty());

    // Test 10: Remove configuration.
    auto removeResult = manager.remove("app.enabled");

    assert(removeResult.hasValue());
    assert(removeResult.value());
    assert(!manager.contains("app.enabled"));

    // Test 11: Remove missing configuration.
    auto missingRemove = manager.remove("missing.key");

    assert(!missingRemove.hasValue());

    // Test 12: Clear manager.
    manager.clear();

    assert(manager.empty());
    assert(manager.size() == 0);
    assert(!manager.contains("app.name"));
    assert(!manager.contains("app.theme"));

    std::cout << "[PASS] ConfigManager tests\n";

    return 0;
}
