#include "nexora/configuration/config_loader.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using namespace nexora::configuration;

// Test parser implementation.
class LoaderTestParser final : public ConfigParser
{
public:
    ConfigResult<ConfigValue> parse(
        std::string_view input) const override
    {
        if (input == "NEXORA")
        {
            return ConfigValue("NEXORA");
        }

        return ConfigError{
            ConfigErrorCode::ParseError,
            "Invalid configuration content"
        };
    }
};

int main()
{
    namespace fs = std::filesystem;

    const fs::path testDirectory =
        fs::temp_directory_path() / "nexora_loader_tests";

    const fs::path testFile =
        testDirectory / "config.txt";

    fs::create_directories(testDirectory);

    // Prepare test configuration file.
    {
        std::ofstream file(testFile);
        assert(file.is_open());

        file << "NEXORA";
    }

    LoaderTestParser parser;

    // Test 1: Load configuration successfully.
    auto loadResult = ConfigLoader::load(testFile, parser);

    assert(loadResult.hasValue());
    assert(loadResult.value().isString());
    assert(*loadResult.value().asString() == "NEXORA");

    // Test 2: Load configuration into an output value.
    ConfigValue output;

    auto loadIntoResult =
        ConfigLoader::loadInto(testFile, parser, output);

    assert(loadIntoResult.hasValue());
    assert(loadIntoResult.value());

    assert(output.isString());
    assert(*output.asString() == "NEXORA");

    // Test 3: Load invalid configuration content.
    {
        std::ofstream file(testFile, std::ios::trunc);
        assert(file.is_open());

        file << "INVALID";
    }

    auto invalidResult = ConfigLoader::load(testFile, parser);

    assert(!invalidResult.hasValue());
    assert(
        invalidResult.error().code() ==
        ConfigErrorCode::ParseError
    );

    // Test 4: Load a missing file.
    const fs::path missingFile =
        testDirectory / "missing.txt";

    auto missingResult = ConfigLoader::load(missingFile, parser);

    assert(!missingResult.hasValue());
    assert(
        missingResult.error().code() ==
        ConfigErrorCode::FileNotFound
    );

    // Test 5: Failed loadInto should preserve existing output.
    ConfigValue preservedOutput("PRESERVED");

    auto failedLoadInto =
        ConfigLoader::loadInto(missingFile, parser, preservedOutput);

    assert(!failedLoadInto.hasValue());
    assert(preservedOutput.isString());
    assert(*preservedOutput.asString() == "PRESERVED");

    // Cleanup.
    fs::remove_all(testDirectory);

    std::cout << "[PASS] ConfigLoader tests\n";

    return 0;
}