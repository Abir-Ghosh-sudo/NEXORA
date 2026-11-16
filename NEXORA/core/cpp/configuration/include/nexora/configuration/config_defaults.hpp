
#pragma once

#include "config_value.hpp"

#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace nexora::configuration
{

class ConfigDefaults
{
public:
    ConfigDefaults() = default;

    ConfigDefaults(const ConfigDefaults&) = delete;
    ConfigDefaults& operator=(const ConfigDefaults&) = delete;

    void set(
        std::string key,
        ConfigValue value
    )
    {
        std::lock_guard<std::mutex> lock(mutex_);

        defaults_.insert_or_assign(
            std::move(key),
            std::move(value)
        );
    }

    [[nodiscard]]
    bool contains(const std::string& key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        return defaults_.find(key) != defaults_.end();
    }

    [[nodiscard]]
    bool get(
        const std::string& key,
        ConfigValue& output
    ) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const auto it = defaults_.find(key);

        if (it == defaults_.end())
        {
            return false;
        }

        output = it->second;
        return true;
    }

    [[nodiscard]]
    std::vector<std::string> keys() const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::vector<std::string> result;
        result.reserve(defaults_.size());

        for (const auto& [key, value] : defaults_)
        {
            (void)value;
            result.push_back(key);
        }

        return result;
    }

    [[nodiscard]]
    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        return defaults_.size();
    }

    [[nodiscard]]
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        return defaults_.empty();
    }

    [[nodiscard]]
    bool remove(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        return defaults_.erase(key) > 0;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        defaults_.clear();
    }

private:
    mutable std::mutex mutex_;

    std::unordered_map<std::string, ConfigValue> defaults_;
};

} // namespace nexora::configuration
