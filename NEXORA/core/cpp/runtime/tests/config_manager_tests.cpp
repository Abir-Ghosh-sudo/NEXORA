
#include <nexora/runtime/config_manager.hpp>

#include <cassert>

using namespace nexora::runtime;

int main() {
    ConfigManager manager;

    // Initially no configuration should be loaded.
    assert(!manager.isLoaded());
    assert(manager.get() == nullptr);

    // Load a valid configuration.
    RuntimeConfig config{};
    config.instance_name = "NEXORA";
    config.version = Version{1, 0, 0};

    const Status loadStatus = manager.load(config);

    assert(loadStatus.isOk());
    assert(manager.isLoaded());
    assert(manager.get() != nullptr);
    assert(manager.get()->instance_name == "NEXORA");

    // Reject invalid configuration.
    RuntimeConfig invalidConfig{};
    invalidConfig.instance_name = "";
    invalidConfig.version = Version{1, 0, 0};

    const Status invalidStatus = manager.load(invalidConfig);

    assert(invalidStatus.hasError());
    assert(manager.isLoaded());
    assert(manager.get()->instance_name == "NEXORA");

    // Reset configuration.
    manager.reset();

    assert(!manager.isLoaded());
    assert(manager.get() == nullptr);

    return 0;
}
