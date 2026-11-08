
# ============================================================
# NEXORA Diagnostics System
# ============================================================

add_library(nexora_diagnostics INTERFACE)

add_library(
    NEXORA::Diagnostics
    ALIAS nexora_diagnostics
)

# ============================================================
# Include directories
# ============================================================

target_include_directories(
    nexora_diagnostics
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ============================================================
# Language standard
# ============================================================

target_compile_features(
    nexora_diagnostics
    INTERFACE
        cxx_std_20
)

# ============================================================
# Project build options
# ============================================================

target_link_libraries(
    nexora_diagnostics
    INTERFACE
        NEXORA::BuildOptions
)

message(STATUS "NEXORA Diagnostics System configured")

