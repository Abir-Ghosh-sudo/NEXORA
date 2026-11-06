
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

#include <nexora/runtime/result.hpp>

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

void testSuccessResult() {
    using namespace nexora::runtime;

    auto result = Result<int>::success(42);

    expect(result.hasValue(), "Success result contains a value");
    expect(!result.hasError(), "Success result has no error");
    expect(static_cast<bool>(result), "Success result converts to true");
    expect(result.value() == 42, "Success value is preserved");
}

void testFailureResult() {
    using namespace nexora::runtime;

    auto result = Result<int>::failure(
        makeError(
            ErrorCode::RuntimeInitializationFailed,
            ErrorCategory::Initialization,
            ErrorSeverity::Error,
            "Initialization failed"
        )
    );

    expect(!result.hasValue(), "Failure result has no value");
    expect(result.hasError(), "Failure result contains an error");
    expect(!static_cast<bool>(result), "Failure result converts to false");

    expect(
        result.error().code == ErrorCode::RuntimeInitializationFailed,
        "Failure error code is preserved"
    );

    expect(
        result.error().message == "Initialization failed",
        "Failure error message is preserved"
    );
}

void testValuePointer() {
    using namespace nexora::runtime;

    auto result = Result<int>::success(100);

    int* value = result.valueIf();

    expect(value != nullptr, "valueIf returns a valid pointer");

    if (value != nullptr) {
        expect(*value == 100, "valueIf provides the stored value");
    }
}

void testErrorPointer() {
    using namespace nexora::runtime;

    auto result = Result<int>::failure(
        makeError(
            ErrorCode::RuntimeInvalidState,
            ErrorCategory::Lifecycle,
            ErrorSeverity::Warning,
            "Invalid runtime state"
        )
    );

    const auto* error = result.errorIf();

    expect(error != nullptr, "errorIf returns a valid pointer");

    if (error != nullptr) {
        expect(
            error->code == ErrorCode::RuntimeInvalidState,
            "errorIf provides the stored error"
        );
    }
}

void testStringResult() {
    using namespace nexora::runtime;

    auto result = Result<std::string>::success("NEXORA");

    expect(
        result.hasValue() && result.value() == "NEXORA",
        "Result supports std::string values"
    );
}

void testMoveResult() {
    using namespace nexora::runtime;

    auto result = Result<std::string>::success("Runtime");

    std::string value = std::move(result).value();

    expect(
        value == "Runtime",
        "Result supports moving its stored value"
    );
}

} // namespace

int main() {
    testSuccessResult();
    testFailureResult();
    testValuePointer();
    testErrorPointer();
    testStringResult();
    testMoveResult();

    std::cout << "\nResult Test Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
