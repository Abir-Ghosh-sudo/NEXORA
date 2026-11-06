
#pragma once

#include <nexora/runtime/error.hpp>

namespace nexora::runtime {

class Status final {
public:
    constexpr Status() noexcept = default;

    [[nodiscard]]
    static constexpr Status success() noexcept {
        return Status{};
    }

    [[nodiscard]]
    static constexpr Status failure(Error error) noexcept {
        return Status{error};
    }

    [[nodiscard]]
    constexpr bool isOk() const noexcept {
        return error_.isOk();
    }

    [[nodiscard]]
    constexpr bool hasError() const noexcept {
        return !isOk();
    }

    [[nodiscard]]
    constexpr explicit operator bool() const noexcept {
        return isOk();
    }

    [[nodiscard]]
    constexpr const Error* errorIf() const noexcept {
        return hasError() ? &error_ : nullptr;
    }

    [[nodiscard]]
    constexpr const Error& error() const noexcept {
        return error_;
    }

private:
    constexpr explicit Status(Error error) noexcept
        : error_(error) {}

    Error error_{};
};

} // namespace nexora::runtime
