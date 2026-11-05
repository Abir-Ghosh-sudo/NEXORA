
#include <cstdlib>
#include <iostream>

#include <nexora/runtime/runtime_info.hpp>

namespace {

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

void testVersion() {
    using namespace nexora::runtime;

    expect(kRuntimeInfo.version == kVersion,
           "Runtime version matches project version");

    expect(kRuntimeInfo.versionString == kVersionString,
           "Runtime version string matches");

    expect(kRuntimeInfo.version.major == 0,
           "Runtime major version is correct");
}

void testPlatform() {
    using namespace nexora::runtime;

    expect(platform() != Platform::Unknown,
           "Platform is detected");

    expect(!platformName().empty(),
           "Platform name is not empty");
}

void testCompiler() {
    using namespace nexora::runtime;

    expect(!compilerName().empty(),
           "Compiler is detected");
}

void testBuildConfiguration() {
    using namespace nexora::runtime;

    expect(
        buildConfiguration() == "Debug" ||
        buildConfiguration() == "Release",
        "Build configuration is recognized"
    );
}

void testCppStandard() {
    using namespace nexora::runtime;

    expect(kRuntimeInfo.cppStandard >= 202002L,
           "C++20 or newer is enabled");
}

} // namespace

int main() {
    testVersion();
    testPlatform();
    testCompiler();
    testBuildConfiguration();
    testCppStandard();

    std::cout << "\nRuntime Information Test Summary\n";
    std::cout << "Passed: " << passed << '\n';
    std::cout << "Failed: " << failed << '\n';

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
