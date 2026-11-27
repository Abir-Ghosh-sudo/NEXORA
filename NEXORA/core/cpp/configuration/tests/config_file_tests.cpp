#include "nexora/configuration/config_file.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>

using namespace nexora::configuration;

int main()
{
    namespace fs = std::filesystem;

    const fs::path testDirectory =
        fs::temp_directory_path() / "nexora_config_tests";

    const fs::path testFile =
        testDirectory / "test_config.json";

    // Prepare test directory.
    fs::create_directories(testDirectory);

    // Test 1: File should not exist initially.
    assert(!ConfigFile::exists(testFile));

    // Test 2: Write content to file.
    const std::string content =
        R"({"app":"NEXORA","version":1})";

    auto writeResult = ConfigFile::write(testFile, content);

    assert(writeResult.hasValue());
    assert(writeResult.value());

    // Test 3: Check file existence.
    assert(ConfigFile::exists(testFile));

    // Test 4: Read file content.
    auto readResult = ConfigFile::read(testFile);

    assert(readResult.hasValue());
    assert(readResult.value() == content);

    // Test 5: Overwrite existing file.
    const std::string updatedContent = "NEXORA CONFIGURATION";

    auto updateResult = ConfigFile::write(
        testFile,
        updatedContent
    );

    assert(updateResult.hasValue());
    assert(updateResult.value());

    auto updatedReadResult = ConfigFile::read(testFile);

    assert(updatedReadResult.hasValue());
    assert(updatedReadResult.value() == updatedContent);

    // Test 6: Read missing file.
    const fs::path missingFile =
        testDirectory / "missing.json";

    auto missingResult = ConfigFile::read(missingFile);

    assert(!missingResult.hasValue());
    assert(
        missingResult.error().code() ==
        ConfigErrorCode::FileNotFound
    );

    // Test 7: Remove existing file.
    auto removeResult = ConfigFile::remove(testFile);

    assert(removeResult.hasValue());
    assert(removeResult.value());
    assert(!ConfigFile::exists(testFile));

    // Test 8: Remove missing file.
    auto removeMissingResult = ConfigFile::remove(missingFile);

    assert(!removeMissingResult.hasValue());

    // Cleanup.
    fs::remove_all(testDirectory);

    std::cout << "[PASS] ConfigFile tests\n";

    return 0;
}