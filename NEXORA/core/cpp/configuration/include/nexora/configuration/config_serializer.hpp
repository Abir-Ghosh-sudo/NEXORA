
#pragma once

#include "config_error.hpp"
#include "config_result.hpp"
#include "config_value.hpp"

#include <string>

namespace nexora::configuration
{

class ConfigSerializer
{
public:
    virtual ~ConfigSerializer() = default;

    ConfigSerializer() = default;

    ConfigSerializer(const ConfigSerializer&) = default;
    ConfigSerializer& operator=(const ConfigSerializer&) = default;

    ConfigSerializer(ConfigSerializer&&) noexcept = default;
    ConfigSerializer& operator=(ConfigSerializer&&) noexcept = default;

    [[nodiscard]]
    virtual ConfigResult<std::string> serialize(
        const ConfigValue& value
    ) const = 0;

};

} // namespace nexora::configuration
