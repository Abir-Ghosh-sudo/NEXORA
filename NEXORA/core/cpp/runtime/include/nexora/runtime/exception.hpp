
#pragma once

#include <exception>
#include <string>
#include <string_view>

#include <nexora/runtime/error.hpp>

namespace nexora::runtime {

class RuntimeException final : public std::exception {
public:
    explicit RuntimeException(const Error& error)
        : code_(error.code),
          category_(error.category),
          severity_(error.severity),
          message_(error.message) {}

    [[nodiscard]]
    const char* what() const noexcept override {
        return message_.c_str();
    }

    [[nodiscard]]
    ErrorCode code() const noexcept {
        return code_;
    }

    [[nodiscard]]
    ErrorCategory category() const noexcept {
        return category_;
    }

    [[nodiscard]]
    ErrorSeverity severity() const noexcept {
        return severity_;
    }

    [[nodiscard]]
    Error error() const noexcept {
        return Error{
            code_,
            category_,
            severity_,
            std::string_view{message_}
        };
    }

private:
    ErrorCode code_;
    ErrorCategory category_;
    ErrorSeverity severity_;
    std::string message_;
};

} // namespace nexora::runtime
