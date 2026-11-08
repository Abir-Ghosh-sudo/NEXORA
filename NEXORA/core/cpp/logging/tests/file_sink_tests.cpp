
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <nexora/logging/file_sink.hpp>

namespace {

using namespace nexora::logging;

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

std::string readFile(const std::filesystem::path& path) {
    std::ifstream input(path);

    if (!input.is_open()) {
        return {};
    }

    std::ostringstream content;
    content << input.rdbuf();

    return content.str();
}

// Test 1: File creation
void testFileCreation() {
    const auto directory =
        std::filesystem::temp_directory_path() /
        "nexora_logging_tests";

    const auto file_path = directory / "creation_test.log";

    std::filesystem::remove_all(directory);

    {
        FileSink sink(file_path);
    }

    expect(
        std::filesystem::exists(file_path),
        "FileSink should create the log file"
    );

    std::filesystem::remove_all(directory);
}

// Test 2: Single message write
void testSingleMessageWrite() {
    const auto file_path =
        std::filesystem::temp_directory_path() /
        "nexora_logging_single_test.log";

    std::filesystem::remove(file_path);

    {
        FileSink sink(file_path);
        sink.write("NEXORA file logging test");
    }

    const std::string content = readFile(file_path);

    expect(
        content.find("NEXORA file logging test") != std::string::npos,
        "FileSink should write a message"
    );

    std::filesystem::remove(file_path);
}

// Test 3: Multiple message append
void testMultipleMessageAppend() {
    const auto file_path =
        std::filesystem::temp_directory_path() /
        "nexora_logging_append_test.log";

    std::filesystem::remove(file_path);

    {
        FileSink sink(file_path);
        sink.write("First message");
        sink.write("Second message");
    }

    const std::string content = readFile(file_path);

    expect(
        content.find("First message") != std::string::npos &&
        content.find("Second message") != std::string::npos,
        "FileSink should append multiple messages"
    );

    std::filesystem::remove(file_path);
}

// Test 4: Nested directory creation
void testNestedDirectoryCreation() {
    const auto directory =
        std::filesystem::temp_directory_path() /
        "nexora_logging_nested" / "logs";

    const auto file_path = directory / "nested.log";

    std::filesystem::remove_all(directory.parent_path());

    {
        FileSink sink(file_path);
        sink.write("Nested directory test");
    }

    expect(
        std::filesystem::exists(file_path),
        "FileSink should create missing parent directories"
    );

    std::filesystem::remove_all(directory.parent_path());
}

// Test 5: Reject an existing directory as a log file
void testInvalidFilePath() {
    const auto invalid_path =
        std::filesystem::temp_directory_path() /
        "nexora_logging_invalid_directory";

    std::filesystem::remove_all(invalid_path);
    std::filesystem::create_directories(invalid_path);

    bool exception_thrown = false;

    try {
        FileSink sink(invalid_path);
    } catch (const std::exception&) {
        exception_thrown = true;
    }

    expect(
        exception_thrown,
        "FileSink should reject an existing directory as a log file"
    );

    std::filesystem::remove_all(invalid_path);
}

} // namespace

int main() {
    testFileCreation();
    testSingleMessageWrite();
    testMultipleMessageAppend();
    testNestedDirectoryCreation();
    testInvalidFilePath();

    std::cout << "\n================================\n";
    std::cout << "FileSink Tests Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';
    std::cout << "================================\n";

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
