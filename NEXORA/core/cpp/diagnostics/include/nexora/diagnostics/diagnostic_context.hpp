
#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>

namespace nexora::diagnostics {

class DiagnosticContext final {
public:

    using Metadata = std::unordered_map<std::string, std::string>;

    void set(
        std::string key,
        std::string value
    ) {
        metadata_.insert_or_assign(
            std::move(key),
            std::move(value)
        );
    }

    [[nodiscard]]
    bool contains(const std::string& key) const {
        return metadata_.find(key) != metadata_.end();
    }

    [[nodiscard]]
    const std::string* get(
        const std::string& key
    ) const noexcept {
        const auto iterator = metadata_.find(key);

        if (iterator == metadata_.end()) {
            return nullptr;
        }

        return &iterator->second;
    }

    [[nodiscard]]
    const Metadata& metadata() const noexcept {
        return metadata_;
    }

    [[nodiscard]]
    bool empty() const noexcept {
        return metadata_.empty();
    }

    [[nodiscard]]
    std::size_t size() const noexcept {
        return metadata_.size();
    }

    void clear() noexcept {
        metadata_.clear();
    }

private:

    Metadata metadata_{};
};

} // namespace nexora::diagnostics
