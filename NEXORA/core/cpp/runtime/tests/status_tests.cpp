
#include <nexora/runtime/status.hpp>

#include <cassert>

using namespace nexora::runtime;

int main() {
    // Test successful status.
    const Status success = Status::success();

    assert(success.isOk());
    assert(!success.hasError());
    assert(static_cast<bool>(success));
    assert(success.errorIf() == nullptr);

    // Test failed status.
    constexpr Error testError = makeError(
        ErrorCode::RuntimeInvalidState,
        ErrorCategory::Lifecycle,
        ErrorSeverity::Error,
        "Runtime is in an invalid state."
    );

    const Status failure = Status::failure(testError);

    assert(!failure.isOk());
    assert(failure.hasError());
    assert(!static_cast<bool>(failure));
    assert(failure.errorIf() != nullptr);
    assert(failure.error().code == ErrorCode::RuntimeInvalidState);

    return 0;
}
