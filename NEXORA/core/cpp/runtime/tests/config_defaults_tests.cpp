#include <nexora/runtime/config_defaults.hpp>
#include <nexora/runtime/config_validator.hpp>

#include <cassert>

using namespace nexora::runtime;

int main() {
    constexpr RuntimeConfig config = ConfigDefaults::create();

    // Check default instance name.
    assert(config.instance_name == "NEXORA");

    // Check default version.
    assert(config.version.major == 1);
    assert(config.version.minor == 0);
    assert(config.version.patch == 0);

    // Check default feature flags.
    assert(config.enable_diagnostics);
    assert(config.enable_plugins);
    assert(!config.enable_ai_services);

    // Check that default configuration is valid.
    const Status status = ConfigValidator::validate(config);

    assert(status.isOk());

    return 0;
}
