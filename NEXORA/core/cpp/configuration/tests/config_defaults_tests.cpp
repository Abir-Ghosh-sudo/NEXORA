
#include "nexora/configuration/config_defaults.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace nexora::configuration;

int main()
{
    ConfigDefaults defaults;

    // 1. Initial state
    assert(defaults.empty());
    assert(defaults.size() == 0);

    // 2. Set default values
    defaults.set("app.name", ConfigValue("NEXORA"));
    defaults.set("app.version", ConfigValue(1));
    defaults.set("app.enabled", ConfigValue(true));

    assert(defaults.size() == 3);
    assert(!defaults.empty());

    // 3. Check existence
    assert(defaults.contains("app.name"));
    assert(defaults.contains("app.version"));
    assert(defaults.contains("app.enabled"));
    assert(!defaults.contains("app.missing"));

    // 4. Retrieve string value
    ConfigValue nameValue;

    const bool nameFound = defaults.get("app.name", nameValue);

    assert(nameFound);
    assert(nameValue.isString());
    assert(*nameValue.asString() == "NEXORA");

    // 5. Retrieve integer value
    ConfigValue versionValue;

    const bool versionFound =
        defaults.get("app.version", versionValue);

    assert(versionFound);
    assert(versionValue.isInteger());
    assert(*versionValue.asInteger() == 1);

    // 6. Retrieve boolean value
    ConfigValue enabledValue;

    const bool enabledFound =
        defaults.get("app.enabled", enabledValue);

    assert(enabledFound);
    assert(enabledValue.isBoolean());
    assert(*enabledValue.asBoolean());

    // 7. Missing key
    ConfigValue missingValue;

    const bool missingFound =
        defaults.get("app.missing", missingValue);

    assert(!missingFound);

    // 8. Update an existing default
    defaults.set("app.name", ConfigValue("NEXORA IDE"));

    ConfigValue updatedName;

    assert(defaults.get("app.name", updatedName));
    assert(*updatedName.asString() == "NEXORA IDE");

    // 9. Retrieve all keys
    const auto keys = defaults.keys();

    assert(keys.size() == 3);

    // 10. Remove a default
    const bool removed = defaults.remove("app.version");

    assert(removed);
    assert(!defaults.contains("app.version"));
    assert(defaults.size() == 2);

    // 11. Remove missing key
    const bool missingRemoval =
        defaults.remove("app.missing");

    assert(!missingRemoval);

    // 12. Clear defaults
    defaults.clear();

    assert(defaults.empty());
    assert(defaults.size() == 0);
    assert(!defaults.contains("app.name"));

    std::cout
        << "[PASS] ConfigDefaults tests completed successfully.\n";

    return 0;
}