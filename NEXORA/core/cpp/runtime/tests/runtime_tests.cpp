
#include <cstdlib>
#include <iostream>

#include <nexora/runtime/runtime.hpp>

namespace {

int testsPassed = 0;
int testsFailed = 0;

void expect(bool condition, const char* description) {
    if (condition) {
        ++testsPassed;
        std::cout << "[PASS] " << description << '\n';
    } else {
        ++testsFailed;
        std::cerr << "[FAIL] " << description << '\n';
    }
}

void testInitialState() {
    using namespace nexora::runtime;

    expect(
        Runtime::state() == RuntimeState::Uninitialized,
        "Runtime starts in Uninitialized state"
    );

    expect(
        !Runtime::isInitialized(),
        "Runtime is not initialized initially"
    );
}

void testInitialization() {
    using namespace nexora::runtime;

    const auto result = Runtime::initialize();

    expect(
        result == RuntimeResult::Success,
        "Runtime initializes successfully"
    );

    expect(
        Runtime::state() == RuntimeState::Initialized,
        "Runtime enters Initialized state"
    );

    expect(
        Runtime::isInitialized(),
        "Runtime reports initialized"
    );

    expect(
        Runtime::initialize() == RuntimeResult::AlreadyInitialized,
        "Repeated initialization is rejected"
    );
}

void testShutdown() {
    using namespace nexora::runtime;

    const auto result = Runtime::shutdown();

    expect(
        result == ShutdownResult::Success,
        "Runtime shuts down successfully"
    );

    expect(
        Runtime::state() == RuntimeState::Stopped,
        "Runtime enters Stopped state"
    );

    expect(
        !Runtime::isInitialized(),
        "Runtime is no longer initialized"
    );

    expect(
        Runtime::shutdown() == ShutdownResult::AlreadyStopped,
        "Repeated shutdown is handled"
    );
}

} // namespace

int main() {
    testInitialState();
    testInitialization();
    testShutdown();

    std::cout << "\nRuntime Test Summary\n";
    std::cout << "Passed: " << testsPassed << '\n';
    std::cout << "Failed: " << testsFailed << '\n';

    return testsFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
