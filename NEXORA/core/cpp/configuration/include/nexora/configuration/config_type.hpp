
#pragma once

#include <cstdint>

namespace nexora::configuration
{

enum class ConfigType : std::uint8_t
{
    Null = 0,
    Boolean,
    Integer,
    FloatingPoint,
    String,
    Array,
    Object
};

} // namespace nexora::configuration
