
cmake_minimum_required(VERSION 3.25)

project(
    NEXORA
    VERSION 0.1.0
    DESCRIPTION "NEXORA Universal Developer Platform"
    LANGUAGES C CXX
)

# ============================================================
# Project Standards
# ============================================================

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================
# Build Options
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
# Build Configuration
# ============================================================

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Build type" FORCE
    )
endif()

set_property(
    CACHE CMAKE_BUILD_TYPE
    PROPERTY STRINGS
        Debug
        Release
        RelWithDebInfo
        MinSizeRel
)

# ============================================================
# Shared Build Options
# ============================================================

add_library(nexora_build_options INTERFACE)

add_library(
    NEXORA::BuildOptions
    ALIAS nexora_build_options
)

target_compile_features(
    nexora_build_options
    INTERFACE
        c_std_17
        cxx_std_20
)

# ============================================================
# Compiler Configuration
# ============================================================

if(MSVC)

    target_compile_options(
        nexora_build_options
        INTERFACE
            /W4
            /permissive-
            /Zc:preprocessor
            /Zc:__cplusplus
            /utf-8
    )

    if(NEXORA_WARNINGS_AS_ERRORS)
        target_compile_options(
            nexora_build_options
            INTERFACE
                /WX
        )
    endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

    target_compile_options(
        nexora_build_options
        INTERFACE
            $<$<COMPILE_LANGUAGE:C,CXX>:-Wall>
            $<$<COMPILE_LANGUAGE:C,CXX>:-Wextra>
            $<$<COMPILE_LANGUAGE:C,CXX>:-Wpedantic>
            $<$<COMPILE_LANGUAGE:C,CXX>:-Wconversion>
            $<$<COMPILE_LANGUAGE:C,CXX>:-Wsign-conversion>
            $<$<COMPILE_LANGUAGE:C,CXX>:-Wshadow>
            $<$<COMPILE_LANGUAGE:C,CXX>:-Wformat=2>
    )

    if(NEXORA_WARNINGS_AS_ERRORS)
        target_compile_options(
            nexora_build_options
            INTERFACE
                -Werror
        )
    endif()

endif()

# ============================================================
# Sanitizers
# ============================================================

if(NEXORA_ENABLE_SANITIZERS)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

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
            "Sanitizers currently require GCC or Clang."
        )

    endif()

endif()

# ============================================================
# Testing Configuration
# ============================================================

if(NEXORA_BUILD_TESTS)

    include(CTest)
    enable_testing()

    message(STATUS "NEXORA testing enabled")

else()

    message(STATUS "NEXORA testing disabled")

endif()

# ============================================================
# Project Information
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
# Core Components
# ============================================================

add_subdirectory(core/cpp/runtime)
add_subdirectory(core/cpp/logging)
add_subdirectory(core/cpp/diagnostics)
# ============================================================
# Future Components
# ============================================================

# Future integration:
# - C++ Core
# - C System Interfaces
# - Python AI Services
# - TypeScript Desktop Platform
# - Mojo Compute Engine

