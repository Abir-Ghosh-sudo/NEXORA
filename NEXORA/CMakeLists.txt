
cmake_minimum_required(VERSION 3.25)

project(
    NEXORA
    VERSION 0.1.0
    DESCRIPTION "NEXORA Universal Developer Platform"
    LANGUAGES C CXX
)

# ============================================================
# Project standards
# ============================================================

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================
# Build options
# ============================================================

option(NEXORA_BUILD_TESTS
    "Build NEXORA tests"
    ON
)

option(NEXORA_ENABLE_SANITIZERS
    "Enable AddressSanitizer and UndefinedBehaviorSanitizer"
    OFF
)

option(NEXORA_WARNINGS_AS_ERRORS
    "Treat compiler warnings as errors"
    OFF
)

# ============================================================
# Build configuration
# ============================================================

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        "Debug"
        CACHE STRING "Build type" FORCE
    )
endif()

set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel
)

# ============================================================
# Shared build configuration
# ============================================================

add_library(nexora_build_options INTERFACE)

add_library(NEXORA::BuildOptions
    ALIAS nexora_build_options
)

target_compile_features(
    nexora_build_options
    INTERFACE
        c_std_17
        cxx_std_20
)

# ============================================================
# Compiler warnings
# ============================================================

if(MSVC)

    target_compile_options(
        nexora_build_options
        INTERFACE
            /W4
            /permissive-
            /Zc:preprocessor
            /utf-8
    )

    if(NEXORA_WARNINGS_AS_ERRORS)
        target_compile_options(
            nexora_build_options
            INTERFACE /WX
        )
    endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"
       AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")

    target_compile_options(
        nexora_build_options
        INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wshadow
            -Wformat=2
            -Wnull-dereference
    )

    if(NEXORA_WARNINGS_AS_ERRORS)
        target_compile_options(
            nexora_build_options
            INTERFACE -Werror
        )
    endif()

endif()

# ============================================================
# Sanitizers (GCC / Clang)
# ============================================================

if(NEXORA_ENABLE_SANITIZERS)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"
       AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")

        target_compile_options(
            nexora_build_options
            INTERFACE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
        )

        target_link_options(
            nexora_build_options
            INTERFACE
                -fsanitize=address,undefined
        )

    else()

        message(FATAL_ERROR
            "NEXORA sanitizers currently require GCC or Clang."
        )

    endif()

endif()

# ============================================================
# Testing
# ============================================================

if(NEXORA_BUILD_TESTS)
    include(CTest)
    enable_testing()
endif()

# ============================================================
# Project information
# ============================================================

message(STATUS "======================================")
message(STATUS "          NEXORA BUILD SYSTEM         ")
message(STATUS "======================================")
message(STATUS "Version       : ${PROJECT_VERSION}")
message(STATUS "C Standard    : C${CMAKE_C_STANDARD}")
message(STATUS "C++ Standard  : C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type    : ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler      : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Tests         : ${NEXORA_BUILD_TESTS}")
message(STATUS "Sanitizers    : ${NEXORA_ENABLE_SANITIZERS}")
message(STATUS "======================================")

# ============================================================
# Future component integration
# ============================================================

# C++ core, C interfaces, and other components
# will be registered here as their build configurations
# are implemented and verified.
