
#pragma once

#include <cstdint>
#include <string_view>

namespace nexora::runtime {

struct Version final {
    std::uint32_t major;
    std::uint32_t minor;
    std::uint32_t patch;

    [[nodiscard]]
    constexpr bool operator==(const Version& other) const noexcept {
        return major == other.major &&
               minor == other.minor &&
               patch == other.patch;
    }

    [[nodiscard]]
    constexpr bool operator!=(const Version& other) const noexcept {
        return !(*this == other);
    }
};

inline constexpr Version kVersion{
    0,
    1,
    0
};

inline constexpr std::string_view kVersionString{
    "0.1.0"
};

[[nodiscard]]
constexpr Version version() noexcept {
    return kVersion;
}

[[nodiscard]]
constexpr bool isVersion(
    std::uint32_t major,
    std::uint32_t minor,
    std::uint32_t patch
) noexcept {
    return kVersion == Version{major, minor, patch};
}

} // namespace nexora::runtime
