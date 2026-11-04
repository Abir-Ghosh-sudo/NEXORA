
#pragma once

#include <type_traits>
#include <utility>
#include <variant>

#include <nexora/runtime/error.hpp>

namespace nexora::runtime {

/**
 * @brief Type-safe result container.
 *
 * Stores either a successful value or an Error.
 *
 * @tparam T Success value type.
 */
template <typename T>
class [[nodiscard]] Result final {

    static_assert(
        !std::is_void_v<T>,
        "Result<void> is not supported."
    );

    static_assert(
        !std::is_reference_v<T>,
        "Result cannot store reference types."
    );

    static_assert(
        !std::is_same_v<std::remove_cv_t<T>, Error>,
        "Result<T> cannot use Error as its value type."
    );

public:

    // ========================================================
    // Factory Methods
    // ========================================================

    [[nodiscard]]
    static Result success(T value) {
        return Result(
            std::in_place_index<0>,
            std::move(value)
        );
    }

    [[nodiscard]]
    static Result failure(Error error) {
        return Result(
            std::in_place_index<1>,
            std::move(error)
        );
    }

    // ========================================================
    // State Inspection
    // ========================================================

    [[nodiscard]]
    bool hasValue() const noexcept {
        return storage_.index() == 0;
    }

    [[nodiscard]]
    bool hasError() const noexcept {
        return storage_.index() == 1;
    }

    [[nodiscard]]
    explicit operator bool() const noexcept {
        return hasValue();
    }

    // ========================================================
    // Safe Value Access
    // ========================================================

    [[nodiscard]]
    T* valueIf() noexcept {
        return std::get_if<0>(&storage_);
    }

    [[nodiscard]]
    const T* valueIf() const noexcept {
        return std::get_if<0>(&storage_);
    }

    /**
     * @brief Accesses the stored value.
     *
     * Precondition: hasValue() must be true.
     */
    [[nodiscard]]
    T& value() & noexcept {
        return *std::get_if<0>(&storage_);
    }

    [[nodiscard]]
    const T& value() const& noexcept {
        return *std::get_if<0>(&storage_);
    }

    [[nodiscard]]
    T&& value() && noexcept {
        return std::move(*std::get_if<0>(&storage_));
    }

    // ========================================================
    // Safe Error Access
    // ========================================================

    [[nodiscard]]
    Error* errorIf() noexcept {
        return std::get_if<1>(&storage_);
    }

    [[nodiscard]]
    const Error* errorIf() const noexcept {
        return std::get_if<1>(&storage_);
    }

    /**
     * @brief Accesses the stored error.
     *
     * Precondition: hasError() must be true.
     */
    [[nodiscard]]
    Error& error() & noexcept {
        return *std::get_if<1>(&storage_);
    }

    [[nodiscard]]
    const Error& error() const& noexcept {
        return *std::get_if<1>(&storage_);
    }

private:

    // ========================================================
    // Internal Storage
    // ========================================================

    using Storage = std::variant<T, Error>;

    Storage storage_;

    explicit Result(
        std::in_place_index_t<0>,
        T value
    )
        : storage_(
            std::in_place_index<0>,
            std::move(value)
        ) {}

    explicit Result(
        std::in_place_index_t<1>,
        Error error
    )
        : storage_(
            std::in_place_index<1>,
            std::move(error)
        ) {}
};

} // namespace nexora::runtime
