
#pragma once

#include <cstdint>
#include <string_view>

namespace nexora::diagnostics {

struct DiagnosticLocation final {

    std::string_view file{};
    std::string_view function{};

    std::uint_least32_t line{0};
    std::uint_least32_t column{0};

    [[nodiscard]]
    constexpr bool isValid() const noexcept {
        return !file.empty();
    }
};

} // namespace nexora::diagnostics
