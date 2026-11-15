
add_library(nexora_diagnostics INTERFACE)

add_library(NEXORA::Diagnostics ALIAS nexora_diagnostics)

target_include_directories(nexora_diagnostics
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(nexora_diagnostics
    INTERFACE
        cxx_std_20
)

target_link_libraries(nexora_diagnostics
    INTERFACE
        NEXORA::BuildOptions
)

message(STATUS "NEXORA Diagnostics System configured")

# ==========================================
# Diagnostics Tests
# ==========================================

if(NEXORA_BUILD_TESTS)

    set(NEXORA_DIAGNOSTICS_TESTS
        diagnostic_severity
        diagnostic_code
        diagnostic_location
        diagnostic_event
        diagnostic_result
        diagnostic_context
        diagnostic_formatter
        diagnostic_collector
        diagnostic_filter
        diagnostic_statistics
        diagnostic_manager
        diagnostic_reporter
        diagnostic_sink
        console_diagnostic_sink
        file_diagnostic_sink
        json_diagnostic_formatter
        json_file_diagnostic_sink
        multi_diagnostic_sink
        diagnostic_dispatcher
        diagnostic_event_factory
        diagnostic_macros
    )

    foreach(test_name IN LISTS NEXORA_DIAGNOSTICS_TESTS)

        set(test_target nexora_${test_name}_tests)

        add_executable(
            ${test_target}
            tests/${test_name}_tests.cpp
        )

        target_link_libraries(
            ${test_target}
            PRIVATE
                NEXORA::Diagnostics
        )

        add_test(
            NAME ${test_target}
            COMMAND ${test_target}
        )

    endforeach()

endif()
