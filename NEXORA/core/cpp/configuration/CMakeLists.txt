
# ============================================================
# NEXORA Configuration System
# ============================================================

# ============================================================
# Configuration Library
# ============================================================

add_library(nexora_configuration INTERFACE)

target_include_directories(
    nexora_configuration
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(
    nexora_configuration
    INTERFACE
        cxx_std_20
)

add_library(
    NEXORA::Configuration
    ALIAS nexora_configuration
)

# ============================================================
# Configuration Tests
# ============================================================

if(NEXORA_BUILD_TESTS)

    message(STATUS "Configuring NEXORA Configuration tests")

    set(NEXORA_CONFIGURATION_TESTS
        config_value
        config_type
        config_error
        config_result
        config_entry
        config_store
        config_defaults
        config_validator
        config_parser
        config_serializer
        json_config_parser
        json_config_serializer
        environment_config
        config_file
        config_loader
        config_watcher
        config_manager
        config_section
        config_schema
        config_snapshot
    )

    foreach(test_name IN LISTS NEXORA_CONFIGURATION_TESTS)

        set(test_source
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/${test_name}_tests.cpp"
        )

        set(test_target
            "nexora_configuration_${test_name}_tests"
        )

        if(NOT EXISTS "${test_source}")
            message(FATAL_ERROR
                "Configuration test source not found: ${test_source}"
            )
        endif()

        add_executable(
            ${test_target}
            "${test_source}"
        )

        target_link_libraries(
            ${test_target}
            PRIVATE
                NEXORA::Configuration
                NEXORA::BuildOptions
        )

        add_test(
            NAME ${test_target}
            COMMAND ${test_target}
        )

    endforeach()

    message(STATUS "Configuration tests configured: 20")

endif()
