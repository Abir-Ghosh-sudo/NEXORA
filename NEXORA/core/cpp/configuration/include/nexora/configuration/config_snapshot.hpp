
#pragma once

#include "config_entry.hpp"
#include "config_error.hpp"
#include "config_result.hpp"
#include "config_value.hpp"

#include <chrono>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace nexora::configuration
{

class ConfigSnapshot
{
public:
    using Storage = std::unordered_map<std::string, ConfigValue>;
    using Timestamp = std::chrono::system_clock::time_point;

    ConfigSnapshot() = default;

    explicit ConfigSnapshot(Storage values)
        : values_(std::move(values)),
          createdAt_(std::chrono::system_clock::now())
    {
    }

    [[nodiscard]]
    ConfigResult<ConfigValue> get(const std::string& key) const
    {
        const auto iterator = values_.find(key);

        if (iterator == values_.end())
        {
            return ConfigError{
                ConfigErrorCode::NotFound,
                "Snapshot configuration key not found: " + key
            };
        }

        return iterator->second;
    }

    [[nodiscard]]
    bool contains(const std::string& key) const
    {
        return values_.find(key) != values_.end();
    }

    [[nodiscard]]
    const Storage& values() const noexcept
    {
        return values_;
    }

    [[nodiscard]]
    std::vector<std::string> keys() const
    {
        std::vector<std::string> result;
        result.reserve(values_.size());

        for (const auto& [key, value] : values_)
        {
            result.push_back(key);
        }

        return result;
    }

    [[nodiscard]]
    std::size_t size() const noexcept
    {
        return values_.size();
    }

    [[nodiscard]]
    bool empty() const noexcept
    {
        return values_.empty();
    }

    [[nodiscard]]
    Timestamp createdAt() const noexcept
    {
        return createdAt_;
    }

    void clear()
    {
        values_.clear();
        createdAt_ = std::chrono::system_clock::now();
    }

    [[nodiscard]]
    ConfigResult<bool> restore(Storage& output) const
    {
        output = values_;
        return true;
    }

    [[nodiscard]]
    static ConfigSnapshot capture(const Storage& values)
    {
        return ConfigSnapshot{values};
    }

private:
    Storage values_;
    Timestamp createdAt_{std::chrono::system_clock::now()};
};

} // namespace nexora::configuration
