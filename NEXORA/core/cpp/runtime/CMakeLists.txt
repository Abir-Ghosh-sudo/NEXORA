
# ============================================================
# NEXORA Runtime
# Core C++ Runtime Library
# ============================================================

add_library(nexora_runtime STATIC)

add_library(
    NEXORA::Runtime
    ALIAS nexora_runtime
)

# ============================================================
# C++ Standard
# ============================================================

target_compile_features(
    nexora_runtime
    PUBLIC
        cxx_std_20
)

set_target_properties(
    nexora_runtime
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
        OUTPUT_NAME nexora_runtime
)

# ============================================================
# Include Directories
# ============================================================

target_include_directories(
    nexora_runtime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# ============================================================
# Shared Build Configuration
# ============================================================

target_link_libraries(
    nexora_runtime
    PRIVATE
        NEXORA::BuildOptions
)

# ============================================================
# Runtime Build Definitions
# ============================================================

target_compile_definitions(
    nexora_runtime
    PRIVATE
        NEXORA_RUNTIME_BUILD
)

# ============================================================
# Source Integration
# ============================================================

# Runtime source files will be registered using target_sources()
# when their implementations are added.

message(STATUS "NEXORA Runtime configuration loaded")
