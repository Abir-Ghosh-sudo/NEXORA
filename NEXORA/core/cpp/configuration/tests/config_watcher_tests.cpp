
#include "nexora/configuration/config_watcher.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

using namespace nexora::configuration;
using namespace std::chrono_literals;

int main()
{
    namespace fs = std::filesystem;

    const fs::path testDirectory =
        fs::temp_directory_path() / "nexora_watcher_tests";

    const fs::path testFile =
        testDirectory / "watcher_config.txt";

    // Prepare test directory.
    fs::create_directories(testDirectory);

    // Create initial configuration file.
    {
        std::ofstream file(testFile);

        assert(file.is_open());
        file << "INITIAL";
    }

    std::atomic<int> callbackCount{0};

    // Test 1: Construct watcher with file path and interval.
    ConfigWatcher watcher(testFile, 100ms);

    assert(!watcher.isRunning());
    assert(watcher.filePath() == testFile);

    // Test 2: Register callback.
    watcher.setCallback(
        [&callbackCount]()
        {
            callbackCount.fetch_add(1);
        }
    );

    // Test 3: Start watcher.
    const bool started = watcher.start();

    assert(started);
    assert(watcher.isRunning());

    // Test 4: Prevent duplicate start.
    const bool secondStart = watcher.start();

    assert(!secondStart);

    // Allow watcher to initialize.
    std::this_thread::sleep_for(300ms);

    // Test 5: Modify configuration file.
    {
        std::ofstream file(testFile, std::ios::trunc);

        assert(file.is_open());
        file << "UPDATED_CONFIGURATION";
    }

    // Test 6: Wait for callback.
    for (int i = 0; i < 50 && callbackCount.load() == 0; ++i)
    {
        std::this_thread::sleep_for(100ms);
    }

    assert(callbackCount.load() > 0);

    // Test 7: Stop watcher.
    watcher.stop();

    assert(!watcher.isRunning());

    // Test 8: Restart watcher.
    const bool restarted = watcher.start();

    assert(restarted);
    assert(watcher.isRunning());

    // Test 9: Stop restarted watcher.
    watcher.stop();

    assert(!watcher.isRunning());

    // Test 10: Starting with a missing file should fail.
    const fs::path missingFile =
        testDirectory / "missing_config.txt";

    ConfigWatcher missingWatcher(missingFile, 100ms);

    const bool missingStart = missingWatcher.start();

    assert(!missingStart);
    assert(!missingWatcher.isRunning());

    // Cleanup.
    fs::remove_all(testDirectory);

    std::cout << "[PASS] ConfigWatcher tests\n";

    return 0;
}
