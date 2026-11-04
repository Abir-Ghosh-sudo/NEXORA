
#pragma once

#include <atomic>
#include <cstdint>

namespace nexora::runtime {

/**
 * @brief Runtime lifecycle states.
 */
enum class RuntimeState : std::uint8_t {
    Uninitialized = 0,
    Initializing,
    Initialized,
    Stopping,
    Stopped,
    Failed
};

/**
 * @brief Runtime initialization result.
 */
enum class RuntimeResult : std::uint8_t {
    Success = 0,
    AlreadyInitialized,
    InvalidState,
    InitializationFailed
};

/**
 * @brief Runtime shutdown result.
 */
enum class ShutdownResult : std::uint8_t {
    Success = 0,
    AlreadyStopped,
    InvalidState
};

/**
 * @brief Main NEXORA Runtime interface.
 *
 * Controls the lifecycle of the NEXORA core runtime.
 */
class Runtime final {
public:

    Runtime() = delete;

    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;

    /**
     * @brief Initializes the NEXORA Runtime.
     */
    [[nodiscard]]
    static RuntimeResult initialize() noexcept;

    /**
     * @brief Shuts down the NEXORA Runtime.
     */
    [[nodiscard]]
    static ShutdownResult shutdown() noexcept;

    /**
     * @brief Returns the current runtime state.
     */
    [[nodiscard]]
    static RuntimeState state() noexcept;

    /**
     * @brief Checks whether the runtime is initialized.
     */
    [[nodiscard]]
    static bool isInitialized() noexcept;

    /**
     * @brief Checks whether the runtime has failed.
     */
    [[nodiscard]]
    static bool hasFailed() noexcept;

private:

    static std::atomic<RuntimeState> state_;
};

} // namespace nexora::runtime
