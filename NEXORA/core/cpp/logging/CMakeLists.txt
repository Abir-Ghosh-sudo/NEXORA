
# ============================================================
# NEXORA Logging System
# Header-only C++20 logging library
# ============================================================

add_library(nexora_logging INTERFACE)

add_library(NEXORA::Logging ALIAS nexora_logging)

target_include_directories(
    nexora_logging
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
    nexora_logging
    INTERFACE
        cxx_std_20
)

target_link_libraries(
    nexora_logging
    INTERFACE
        NEXORA::BuildOptions
)

# ============================================================
# Logging Tests
# ============================================================

if(NEXORA_BUILD_TESTS)

    function(add_nexora_logging_test target source test_name)

        add_executable(
            ${target}
            ${source}
        )

        target_link_libraries(
            ${target}
            PRIVATE
                NEXORA::Logging
                NEXORA::BuildOptions
        )

        add_test(
            NAME ${test_name}
            COMMAND ${target}
        )

    endfunction()

    add_nexora_logging_test(
        nexora_log_level_tests
        tests/log_level_tests.cpp
        NEXORA.Logging.LogLevel
    )

    add_nexora_logging_test(
        nexora_log_record_tests
        tests/log_record_tests.cpp
        NEXORA.Logging.LogRecord
    )

    add_nexora_logging_test(
        nexora_log_formatter_tests
        tests/log_formatter_tests.cpp
        NEXORA.Logging.Formatter
    )

    add_nexora_logging_test(
        nexora_console_sink_tests
        tests/console_sink_tests.cpp
        NEXORA.Logging.ConsoleSink
    )

    add_nexora_logging_test(
        nexora_file_sink_tests
        tests/file_sink_tests.cpp
        NEXORA.Logging.FileSink
    )

    add_nexora_logging_test(
        nexora_logger_tests
        tests/logger_tests.cpp
        NEXORA.Logging.Logger
    )

endif()
