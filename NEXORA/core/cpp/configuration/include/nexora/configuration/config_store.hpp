
#pragma once

#include "config_entry.hpp"
#include "config_error.hpp"

#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace nexora::configuration
{

class ConfigStore
{
public:
    ConfigStore() = default;

    ConfigStore(const ConfigStore&) = delete;
    ConfigStore& operator=(const ConfigStore&) = delete;

    [[nodiscard]]
    bool contains(const std::string& key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return entries_.find(key) != entries_.end();
    }

    [[nodiscard]]
    ConfigResult<ConfigValue> get(const std::string& key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const auto it = entries_.find(key);

        if (it == entries_.end())
        {
            return ConfigError{
                ConfigErrorCode::NotFound,
                "Configuration key not found: " + key
            };
        }

        return it->second.value();
    }

    [[nodiscard]]
    ConfigResult<ConfigEntry> getEntry(const std::string& key) const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const auto it = entries_.find(key);

        if (it == entries_.end())
        {
            return ConfigError{
                ConfigErrorCode::NotFound,
                "Configuration key not found: " + key
            };
        }

        return it->second;
    }

    [[nodiscard]]
    ConfigResult<bool> insert(ConfigEntry entry)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const std::string key = entry.key();

        if (entries_.find(key) != entries_.end())
        {
            return ConfigError{
                ConfigErrorCode::DuplicateKey,
                "Configuration key already exists: " + key
            };
        }

        entries_.emplace(key, std::move(entry));
        return true;
    }

    [[nodiscard]]
    ConfigResult<bool> set(
        const std::string& key,
        ConfigValue value
    )
    {
        std::lock_guard<std::mutex> lock(mutex_);

        const auto it = entries_.find(key);

        if (it == entries_.end())
        {
            return ConfigError{
                ConfigErrorCode::NotFound,
                "Configuration key not found: " + key
            };
        }

        it->second.setValue(std::move(value));
        return true;
    }

    [[nodiscard]]
    bool remove(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return entries_.erase(key) > 0;
    }

    [[nodiscard]]
    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return entries_.size();
    }

    [[nodiscard]]
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return entries_.empty();
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        entries_.clear();
    }

    [[nodiscard]]
    std::vector<std::string> keys() const
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::vector<std::string> result;
        result.reserve(entries_.size());

        for (const auto& [key, entry] : entries_)
        {
            (void)entry;
            result.push_back(key);
        }

        return result;
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, ConfigEntry> entries_;
};

} // namespace nexora::configuration
