
#include <nexora/runtime/config_validator.hpp>

#include <cassert>

using namespace nexora::runtime;

int main() {
    // Valid configuration.
    RuntimeConfig validConfig{};
    validConfig.instance_name = "NEXORA";
    validConfig.version = Version{1, 0, 0};
    validConfig.enable_diagnostics = true;
    validConfig.enable_plugins = true;
    validConfig.enable_ai_services = false;

    const Status validStatus =
        ConfigValidator::validate(validConfig);

    assert(validStatus.isOk());

    // Empty instance name.
    RuntimeConfig emptyNameConfig{};
    emptyNameConfig.instance_name = "";
    emptyNameConfig.version = Version{1, 0, 0};

    const Status emptyNameStatus =
        ConfigValidator::validate(emptyNameConfig);

    assert(emptyNameStatus.hasError());
    assert(
        emptyNameStatus.error().code ==
        ErrorCode::ConfigurationInvalid
    );

    // Invalid version.
    RuntimeConfig invalidVersionConfig{};
    invalidVersionConfig.instance_name = "NEXORA";
    invalidVersionConfig.version = Version{0, 0, 0};

    const Status invalidVersionStatus =
        ConfigValidator::validate(invalidVersionConfig);

    assert(invalidVersionStatus.hasError());
    assert(
        invalidVersionStatus.error().code ==
        ErrorCode::ConfigurationInvalid
    );

    return 0;
}
