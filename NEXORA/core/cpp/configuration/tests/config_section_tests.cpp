
#include "nexora/configuration/config_section.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace nexora::configuration;

int main()
{
    ConfigSection section("database");

    // Test 1: Initial state.
    assert(section.empty());
    assert(section.size() == 0);

    // Test 2: Section name.
    assert(section.name() == "database");

    // Test 3: Insert configuration values.
    auto hostResult = section.set(
        "host",
        ConfigValue("localhost")
    );

    assert(hostResult.hasValue());
    assert(hostResult.value());

    auto portResult = section.set(
        "port",
        ConfigValue(5432)
    );

    assert(portResult.hasValue());
    assert(portResult.value());

    // Test 4: Retrieve values.
    auto host = section.get("host");

    assert(host.hasValue());
    assert(host.value().isString());
    assert(*host.value().asString() == "localhost");

    auto port = section.get("port");

    assert(port.hasValue());
    assert(port.value().isInteger());
    assert(*port.value().asInteger() == 5432);

    // Test 5: Check existence.
    assert(section.contains("host"));
    assert(section.contains("port"));
    assert(!section.contains("username"));

    // Test 6: Update value.
    auto updateResult = section.set(
        "port",
        ConfigValue(3306)
    );

    assert(updateResult.hasValue());
    assert(updateResult.value());

    auto updatedPort = section.get("port");

    assert(updatedPort.hasValue());
    assert(*updatedPort.value().asInteger() == 3306);

    // Test 7: Retrieve missing key.
    auto missing = section.get("username");

    assert(!missing.hasValue());
    assert(missing.error().code() == ConfigErrorCode::NotFound);

    // Test 8: Keys and size.
    auto keys = section.keys();

    assert(keys.size() == 2);
    assert(section.size() == 2);
    assert(!section.empty());

    // Test 9: Remove existing key.
    const bool removed = section.remove("host");

    assert(removed);
    assert(!section.contains("host"));
    assert(section.size() == 1);

    // Test 10: Remove missing key.
    const bool missingRemoval = section.remove("username");

    assert(!missingRemoval);

    // Test 11: Clear section.
    section.clear();

    assert(section.empty());
    assert(section.size() == 0);
    assert(!section.contains("port"));

    std::cout << "[PASS] ConfigSection tests\n";

    return 0;
}
