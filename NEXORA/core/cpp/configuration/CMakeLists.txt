
# ============================================================
# NEXORA Configuration System
# ============================================================

add_library(nexora_configuration INTERFACE)

# Public include directories
target_include_directories(nexora_configuration
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# C++ Standard
target_compile_features(nexora_configuration
    INTERFACE
        cxx_std_20
)

# Namespaced target
add_library(NEXORA::Configuration ALIAS nexora_configuration)
