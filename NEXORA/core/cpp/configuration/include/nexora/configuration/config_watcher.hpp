
#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>

namespace nexora::configuration
{

class ConfigWatcher
{
public:
    using Callback = std::function<void()>;
    using Duration = std::chrono::milliseconds;

    explicit ConfigWatcher(
        std::filesystem::path filePath,
        Duration interval = Duration{500}
    )
        : filePath_(std::move(filePath)),
          interval_(interval.count() > 0
              ? interval
              : Duration{500})
    {
    }

    ConfigWatcher(const ConfigWatcher&) = delete;
    ConfigWatcher& operator=(const ConfigWatcher&) = delete;

    ConfigWatcher(ConfigWatcher&&) = delete;
    ConfigWatcher& operator=(ConfigWatcher&&) = delete;

    ~ConfigWatcher()
    {
        stop();
    }

    void setCallback(Callback callback)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        callback_ = std::move(callback);
    }

    [[nodiscard]]
    bool start()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (running_.load())
        {
            return false;
        }

        if (worker_.joinable())
        {
            return false;
        }

        std::error_code error;

        lastWriteTime_ =
            std::filesystem::last_write_time(filePath_, error);

        if (error)
        {
            return false;
        }

        stopRequested_.store(false);
        running_.store(true);

        try
        {
            worker_ = std::thread(
                &ConfigWatcher::watchLoop,
                this
            );
        }
        catch (...)
        {
            running_.store(false);
            stopRequested_.store(true);
            return false;
        }

        return true;
    }

    void stop()
    {
        stopRequested_.store(true);
        condition_.notify_all();

        if (worker_.joinable())
        {
            if (worker_.get_id() == std::this_thread::get_id())
            {
                return;
            }

            worker_.join();
        }

        running_.store(false);
    }

    [[nodiscard]]
    bool isRunning() const noexcept
    {
        return running_.load();
    }

    [[nodiscard]]
    const std::filesystem::path& filePath() const noexcept
    {
        return filePath_;
    }

private:
    void watchLoop()
    {
        while (!stopRequested_.load())
        {
            {
                std::unique_lock<std::mutex> lock(waitMutex_);

                condition_.wait_for(
                    lock,
                    interval_,
                    [this]()
                    {
                        return stopRequested_.load();
                    }
                );
            }

            if (stopRequested_.load())
            {
                break;
            }

            std::error_code error;

            const auto currentWriteTime =
                std::filesystem::last_write_time(filePath_, error);

            if (error)
            {
                continue;
            }

            bool changed = false;

            {
                std::lock_guard<std::mutex> lock(mutex_);

                if (currentWriteTime != lastWriteTime_)
                {
                    lastWriteTime_ = currentWriteTime;
                    changed = true;
                }
            }

            if (changed)
            {
                Callback callback;

                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    callback = callback_;
                }

                if (callback)
                {
                    try
                    {
                        callback();
                    }
                    catch (...)
                    {
                        // Prevent callback exceptions from terminating
                        // the watcher thread.
                    }
                }
            }
        }

        running_.store(false);
    }

private:
    std::filesystem::path filePath_;
    Duration interval_;

    std::atomic<bool> running_{false};
    std::atomic<bool> stopRequested_{true};

    std::mutex mutex_;
    std::mutex waitMutex_;
    std::condition_variable condition_;

    std::thread worker_;

    std::filesystem::file_time_type lastWriteTime_{};
    Callback callback_;
};

} // namespace nexora::configuration
