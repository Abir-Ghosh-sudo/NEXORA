
#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>

namespace nexora::logging {

class FileSink final {
public:
    explicit FileSink(const std::filesystem::path& file_path)
        : file_path_(file_path) {

        if (!file_path_.parent_path().empty()) {
            std::filesystem::create_directories(
                file_path_.parent_path()
            );
        }

        stream_.open(
            file_path_,
            std::ios::out | std::ios::app
        );

        if (!stream_.is_open()) {
            throw std::runtime_error(
                "NEXORA: Failed to open log file."
            );
        }
    }

    FileSink(const FileSink&) = delete;
    FileSink& operator=(const FileSink&) = delete;

    void write(std::string_view message) {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!stream_.is_open()) {
            throw std::runtime_error(
                "NEXORA: Log file is not open."
            );
        }

        stream_ << message << '\n';

        if (!stream_) {
            throw std::runtime_error(
                "NEXORA: Failed to write log message."
            );
        }

        stream_.flush();
    }

    [[nodiscard]]
    const std::filesystem::path& filePath() const noexcept {
        return file_path_;
    }

private:
    std::filesystem::path file_path_;
    std::ofstream stream_;
    std::mutex mutex_;
};

} // namespace nexora::logging
