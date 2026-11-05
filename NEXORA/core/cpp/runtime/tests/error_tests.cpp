
#include <cstdlib>
#include <iostream>

#include <nexora/runtime/error.hpp>

namespace {

int passed = 0;
int failed = 0;

void expect(bool condition, const char* message) {
    if (condition) {
        ++passed;
        std::cout << "[PASS] " << message << '\n';
    } else {
        ++failed;
        std::cerr << "[FAIL] " << message << '\n';
    }
}

void testErrorCreation() {
    using namespace nexora::runtime;

    constexpr Error error = makeError(
        ErrorCode::RuntimeInitializationFailed,
        ErrorCategory::Initialization,
        ErrorSeverity::Error,
        "Runtime initialization failed"
    );

    expect(!error.isOk(), "Error is not marked as successful");
    expect(static_cast<bool>(error), "Error converts to true");
    expect(
        error.message == "Runtime initialization failed",
        "Error message is preserved"
    );
}

void testErrorMetadata() {
    using namespace nexora::runtime;

    constexpr Error error = makeError(
        ErrorCode::RuntimeInitializationFailed,
        ErrorCategory::Initialization,
        ErrorSeverity::Critical,
        "Initialization failure"
    );

    expect(
        error.category == ErrorCategory::Initialization,
        "Error category is preserved"
    );

    expect(
        error.severity == ErrorSeverity::Critical,
        "Error severity is preserved"
    );

    expect(
        error.code == ErrorCode::RuntimeInitializationFailed,
        "Error code is preserved"
    );
}

void testErrorNames() {
    using namespace nexora::runtime;

    expect(
        errorCodeName(ErrorCode::RuntimeInitializationFailed) ==
            "RuntimeInitializationFailed",
        "Error code name is correct"
    );

    expect(
        categoryName(ErrorCategory::Initialization) ==
            "Initialization",
        "Error category name is correct"
    );

    expect(
        severityName(ErrorSeverity::Warning) == "Warning",
        "Error severity name is correct"
    );
}

void testSuccessError() {
    using namespace nexora::runtime;

    constexpr Error error = makeError(
        ErrorCode::None,
        ErrorCategory::None,
        ErrorSeverity::Trace,
        ""
    );

    expect(error.isOk(), "None error code represents success");
    expect(!static_cast<bool>(error), "Success converts to false");
}

} // namespace

int main() {
    testErrorCreation();
    testErrorMetadata();
    testErrorNames();
    testSuccessError();

    std::cout << "\nError Test Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
