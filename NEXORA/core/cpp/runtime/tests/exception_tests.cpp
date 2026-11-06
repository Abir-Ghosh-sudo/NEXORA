
#include <cstdlib>
#include <iostream>
#include <string_view>

#include <nexora/runtime/exception.hpp>

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

void testExceptionMessage() {
    using namespace nexora::runtime;

    const RuntimeException exception{
        makeError(
            ErrorCode::RuntimeInitializationFailed,
            ErrorCategory::Initialization,
            ErrorSeverity::Critical,
            "Runtime initialization failed"
        )
    };

    expect(
        std::string_view{exception.what()} ==
            "Runtime initialization failed",
        "Exception message is preserved"
    );
}

void testExceptionMetadata() {
    using namespace nexora::runtime;

    const RuntimeException exception{
        makeError(
            ErrorCode::RuntimeInvalidState,
            ErrorCategory::Lifecycle,
            ErrorSeverity::Error,
            "Invalid runtime state"
        )
    };

    expect(
        exception.code() == ErrorCode::RuntimeInvalidState,
        "Exception error code is preserved"
    );

    expect(
        exception.category() == ErrorCategory::Lifecycle,
        "Exception category is preserved"
    );

    expect(
        exception.severity() == ErrorSeverity::Error,
        "Exception severity is preserved"
    );
}

void testErrorConversion() {
    using namespace nexora::runtime;

    const RuntimeException exception{
        makeError(
            ErrorCode::InternalError,
            ErrorCategory::Internal,
            ErrorSeverity::Fatal,
            "Internal runtime failure"
        )
    };

    const Error error = exception.error();

    expect(
        error.code == ErrorCode::InternalError,
        "Exception converts back to Error"
    );

    expect(
        error.message == "Internal runtime failure",
        "Converted Error retains its message"
    );

    expect(
        error.severity == ErrorSeverity::Fatal,
        "Converted Error retains its severity"
    );
}

} // namespace

int main() {
    testExceptionMessage();
    testExceptionMetadata();
    testErrorConversion();

    std::cout << "\nException Test Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
