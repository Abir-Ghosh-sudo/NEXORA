
#include "nexora/configuration/config_entry.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using namespace nexora::configuration;

int main()
{
    // Default construction
    ConfigEntry defaultEntry;

    assert(defaultEntry.key().empty());
    assert(defaultEntry.value().isNull());
    assert(defaultEntry.description().empty());

    // Parameterized construction
    ConfigEntry entry(
        "app.name",
        ConfigValue("NEXORA"),
        "Application name"
    );

    assert(entry.key() == "app.name");
    assert(entry.value().isString());
    assert(*entry.value().asString() == "NEXORA");
    assert(entry.description() == "Application name");

    // Timestamp validation
    const auto created = entry.createdAt();
    const auto updated = entry.updatedAt();

    assert(created.time_since_epoch().count() > 0);
    assert(updated >= created);

    // Update value
    entry.setValue(ConfigValue("NEXORA IDE"));

    assert(entry.value().isString());
    assert(*entry.value().asString() == "NEXORA IDE");

    // Updated timestamp
    assert(entry.updatedAt() >= created);

    // Update description
    entry.setDescription("Updated application description");

    assert(
        entry.description() == "Updated application description"
    );

    // Integer value
    ConfigEntry versionEntry(
        "app.version",
        ConfigValue(1),
        "Application version"
    );

    assert(versionEntry.value().isInteger());
    assert(*versionEntry.value().asInteger() == 1);

    std::cout
        << "[PASS] ConfigEntry tests completed successfully.\n";

    return 0;
}
