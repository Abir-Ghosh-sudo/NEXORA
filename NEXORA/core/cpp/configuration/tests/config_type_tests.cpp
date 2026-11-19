
#include "nexora/configuration/config_type.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <type_traits>

using namespace nexora::configuration;

int main()
{
    static_assert(
        std::is_enum_v<ConfigType>,
        "ConfigType must be an enum."
    );

    static_assert(
        std::is_same_v<
            std::underlying_type_t<ConfigType>,
            std::uint8_t
        >,
        "ConfigType must use uint8_t as its underlying type."
    );

    assert(static_cast<std::uint8_t>(ConfigType::Null) == 0);
    assert(static_cast<std::uint8_t>(ConfigType::Boolean) == 1);
    assert(static_cast<std::uint8_t>(ConfigType::Integer) == 2);
    assert(static_cast<std::uint8_t>(ConfigType::FloatingPoint) == 3);
    assert(static_cast<std::uint8_t>(ConfigType::String) == 4);
    assert(static_cast<std::uint8_t>(ConfigType::Array) == 5);
    assert(static_cast<std::uint8_t>(ConfigType::Object) == 6);

    std::cout
        << "[PASS] ConfigType tests completed successfully.\n";

    return 0;
}
