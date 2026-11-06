
#include <nexora/runtime/config_manager.hpp>

#include <cassert>
#include <string>

using namespace nexora::runtime;

int main() {
    ConfigManager manager;

    assert(!manager.isLoaded());
    assert(manager.get() == nullptr);

    // Load configuration from a temporary owned string.
    std::string externalName = "NEXORA";

    RuntimeConfig config{};
    config.instance_name = externalName;
    config.version = Version{1, 0, 0};

    const Status loadStatus = manager.load(config);

    assert(loadStatus.isOk());
    assert(manager.isLoaded());

    externalName = "CHANGED";
    externalName.clear();

    // Manager must retain its own copy.
    assert(manager.get() != nullptr);
    assert(manager.get()->instance_name == "NEXORA");

    // Invalid reload must preserve previous configuration.
    RuntimeConfig invalidConfig{};
    invalidConfig.instance_name = "";
    invalidConfig.version = Version{1, 0, 0};

    const Status invalidStatus = manager.load(invalidConfig);

    assert(invalidStatus.hasError());
    assert(manager.get()->instance_name == "NEXORA");

    // Reset manager.
    manager.reset();

    assert(!manager.isLoaded());
    assert(manager.get() == nullptr);

    return 0;
}
