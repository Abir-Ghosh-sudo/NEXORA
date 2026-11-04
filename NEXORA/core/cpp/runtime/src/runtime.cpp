
#include <nexora/runtime/runtime.hpp>

namespace nexora::runtime {

// ============================================================
// Static Runtime State
// ============================================================

std::atomic<RuntimeState> Runtime::state_{
    RuntimeState::Uninitialized
};

// ============================================================
// Runtime Initialization
// ============================================================

RuntimeResult Runtime::initialize() noexcept {

    RuntimeState expected = RuntimeState::Uninitialized;

    if (!state_.compare_exchange_strong(
            expected,
            RuntimeState::Initializing,
            std::memory_order_acq_rel,
            std::memory_order_acquire
        )) {

        if (expected == RuntimeState::Initialized) {
            return RuntimeResult::AlreadyInitialized;
        }

        return RuntimeResult::InvalidState;
    }

    // Future subsystem initialization will be performed here.
    // Examples:
    // - Configuration Manager
    // - Logging System
    // - Module Manager
    // - Event System

    state_.store(
        RuntimeState::Initialized,
        std::memory_order_release
    );

    return RuntimeResult::Success;
}

// ============================================================
// Runtime Shutdown
// ============================================================

ShutdownResult Runtime::shutdown() noexcept {

    RuntimeState expected = RuntimeState::Initialized;

    if (!state_.compare_exchange_strong(
            expected,
            RuntimeState::Stopping,
            std::memory_order_acq_rel,
            std::memory_order_acquire
        )) {

        if (expected == RuntimeState::Stopped ||
            expected == RuntimeState::Uninitialized) {

            return ShutdownResult::AlreadyStopped;
        }

        return ShutdownResult::InvalidState;
    }

    // Future subsystem shutdown will be performed here.
    // Shutdown order must be reverse of initialization order.

    state_.store(
        RuntimeState::Stopped,
        std::memory_order_release
    );

    return ShutdownResult::Success;
}

// ============================================================
// Runtime State
// ============================================================

RuntimeState Runtime::state() noexcept {

    return state_.load(std::memory_order_acquire);
}

// ============================================================
// Initialization Status
// ============================================================

bool Runtime::isInitialized() noexcept {

    return state() == RuntimeState::Initialized;
}

// ============================================================
// Failure Status
// ============================================================

bool Runtime::hasFailed() noexcept {

    return state() == RuntimeState::Failed;
}

} // namespace nexora::runtime
