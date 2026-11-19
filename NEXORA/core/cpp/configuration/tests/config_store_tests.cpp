
#include "nexora/configuration/config_store.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace nexora::configuration;

int main()
{
    ConfigStore store;

    // 1. Initial state
    assert(store.empty());
    assert(store.size() == 0);

    // 2. Insert an entry
    auto insertResult = store.insert(
        ConfigEntry(
            "app.name",
            ConfigValue("NEXORA"),
            "Application name"
        )
    );

    assert(insertResult.hasValue());
    assert(insertResult.value());

    assert(store.contains("app.name"));
    assert(store.size() == 1);

    // 3. Retrieve a value
    auto getResult = store.get("app.name");

    assert(getResult.hasValue());
    assert(getResult.value().isString());
    assert(*getResult.value().asString() == "NEXORA");

    // 4. Retrieve a complete entry
    auto entryResult = store.getEntry("app.name");

    assert(entryResult.hasValue());
    assert(entryResult.value().key() == "app.name");
    assert(entryResult.value().description() == "Application name");

    // 5. Update an existing value
    auto setResult = store.set(
        "app.name",
        ConfigValue("NEXORA IDE")
    );

    assert(setResult.hasValue());
    assert(setResult.value());

    auto updatedResult = store.get("app.name");

    assert(updatedResult.hasValue());
    assert(updatedResult.value().isString());
    assert(*updatedResult.value().asString() == "NEXORA IDE");

    // 6. Retrieve a missing key
    auto missingResult = store.get("missing.key");

    assert(!missingResult.hasValue());
    assert(
        missingResult.error().code() ==
        ConfigErrorCode::NotFound
    );

    // 7. Retrieve keys
    auto keys = store.keys();

    assert(keys.size() == 1);
    assert(keys.front() == "app.name");

    // 8. Remove an entry
    const bool removed = store.remove("app.name");

    assert(removed);
    assert(!store.contains("app.name"));
    assert(store.empty());

    // 9. Remove a missing entry
    const bool missingRemoval = store.remove("missing.key");

    assert(!missingRemoval);

    // 10. Insert multiple entries
    auto firstInsert = store.insert(
        ConfigEntry("app.version", ConfigValue(1))
    );

    auto secondInsert = store.insert(
        ConfigEntry("app.enabled", ConfigValue(true))
    );

    assert(firstInsert.hasValue());
    assert(secondInsert.hasValue());
    assert(store.size() == 2);

    // 11. Verify stored values
    auto versionResult = store.get("app.version");

    assert(versionResult.hasValue());
    assert(versionResult.value().isInteger());
    assert(*versionResult.value().asInteger() == 1);

    auto enabledResult = store.get("app.enabled");

    assert(enabledResult.hasValue());
    assert(enabledResult.value().isBoolean());
    assert(*enabledResult.value().asBoolean());

    // 12. Clear store
    store.clear();

    assert(store.empty());
    assert(store.size() == 0);
    assert(!store.contains("app.version"));
    assert(!store.contains("app.enabled"));

    std::cout
        << "[PASS] ConfigStore tests completed successfully.\n";

    return 0;
}
