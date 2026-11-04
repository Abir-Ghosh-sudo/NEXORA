
#pragma once

#include <cstdint>
#include <string_view>

#include <nexora/runtime/version.hpp>

namespace nexora::runtime {

// ============================================================
// Platform Information
// ============================================================

enum class Platform : std::uint8_t {
    Unknown = 0,
    Windows,
    Linux,
    MacOS
};

[[nodiscard]]
constexpr Platform platform() noexcept {

#if defined(_WIN32)
    return Platform::Windows;

#elif defined(__linux__)
    return Platform::Linux;

#elif defined(__APPLE__) && defined(__MACH__)
    return Platform::MacOS;

#else
    return Platform::Unknown;
#endif
}

// ============================================================
// Platform Name
// ============================================================

[[nodiscard]]
constexpr std::string_view platformName() noexcept {

    switch (platform()) {

        case Platform::Windows:
            return "Windows";

        case Platform::Linux:
            return "Linux";

        case Platform::MacOS:
            return "macOS";

        default:
            return "Unknown";
    }
}

// ============================================================
// Compiler Information
// ============================================================

[[nodiscard]]
constexpr std::string_view compilerName() noexcept {

#if defined(_MSC_VER)
    return "MSVC";

#elif defined(__clang__)
    return "Clang";

#elif defined(__GNUC__)
    return "GCC";

#else
    return "Unknown";
#endif
}

// ============================================================
// Build Configuration
// ============================================================

[[nodiscard]]
constexpr std::string_view buildConfiguration() noexcept {

#ifdef NDEBUG
    return "Release";
#else
    return "Debug";
#endif
}

// ============================================================
// Runtime Information
// ============================================================

struct RuntimeInfo final {

    std::string_view name;
    Version version;

    std::string_view versionString;
    std::string_view platform;
    std::string_view compiler;
    std::string_view configuration;

    std::uint32_t cppStandard;
};

// ============================================================
// Runtime Metadata
// ============================================================

inline constexpr RuntimeInfo kRuntimeInfo{
    "NEXORA Runtime",
    kVersion,
    kVersionString,
    platformName(),
    compilerName(),
    buildConfiguration(),
    __cplusplus
};

// ============================================================
// Public Information Accessor
// ============================================================

[[nodiscard]]
constexpr const RuntimeInfo& info() noexcept {
    return kRuntimeInfo;
}

} // namespace nexora::runtime
