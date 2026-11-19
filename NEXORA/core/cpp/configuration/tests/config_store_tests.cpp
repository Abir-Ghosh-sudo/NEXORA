
#include "nexora/configuration/config_store.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace nexora::configuration;

int main()
{
    ConfigStore store;

    // Initially empty
    assert(store.empty());
    assert(store.size() == 0);

    // Insert entries
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

    // Retrieve value
    auto getResult = store.get("app.name");

    assert(getResult.hasValue());
    assert(getResult.value().isString());
    assert(*getResult.value().asString() == "NEXORA");

    // Retrieve complete entry
    auto entryResult = store.getEntry("app.name");

    assert(entryResult.hasValue());
    assert(entryResult.value().key() == "app.name");

    // Update existing value
    auto setResult = store.set(
        "app.name",
        ConfigValue("NEXORA IDE")
    );

    assert(setResult.hasValue());
    assert(setResult.value());

    auto updatedResult = store.get("app.name");

    assert(updatedResult.hasValue());
    assert(*updatedResult.value().asString() == "NEXORA IDE");

    // Missing key
    auto missingResult = store.get("missing.key");

    assert(!missingResult.hasValue());
    assert(
        missingResult.error().code() == ConfigErrorCode::NotFound
    );

    // Keys
    auto keys = store.keys();

    assert(keys.size() == 1);
    assert(keys.front() == "app.name");

    // Remove entry
    auto removeResult = store.remove("app.name");

    assert(removeResult.hasValue());
    assert(!store.contains("app.name"));
    assert(store.empty());

    // Clear store
    store.insert(
        ConfigEntry("app.version", ConfigValue(1))
    );

    store.clear();

    assert(store.empty());
    assert(store.size() == 0);

    std::cout
        << "[PASS] ConfigStore tests completed successfully.\n";

    return 0;
}
