
add_library(nexora_runtime STATIC
    src/runtime.cpp
)

add_library(NEXORA::Runtime ALIAS nexora_runtime)

target_compile_features(nexora_runtime PUBLIC
    cxx_std_20
)

target_include_directories(nexora_runtime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(nexora_runtime
    PRIVATE
        NEXORA::BuildOptions
)

set_target_properties(nexora_runtime PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
    OUTPUT_NAME nexora_runtime
)

target_compile_definitions(nexora_runtime
    PRIVATE
        NEXORA_RUNTIME_BUILD
)

message(STATUS "NEXORA Runtime configuration loaded")
if(NEXORA_BUILD_TESTS)

    add_executable(nexora_runtime_info_tests
    tests/runtime_info_tests.cpp
)

target_link_libraries(nexora_runtime_info_tests
    PRIVATE
        NEXORA::Runtime
        NEXORA::BuildOptions
)

add_test(
    NAME NEXORA.Runtime.Information
    COMMAND nexora_runtime_info_tests
)
add_executable(nexora_runtime_info_tests
    tests/runtime_info_tests.cpp
)

target_link_libraries(nexora_runtime_info_tests
    PRIVATE
        NEXORA::Runtime
        NEXORA::BuildOptions
)

add_test(
    NAME NEXORA.Runtime.Information
    COMMAND nexora_runtime_info_tests
)
add_executable(nexora_error_tests
    tests/error_tests.cpp
)

target_link_libraries(nexora_error_tests
    PRIVATE
        NEXORA::Runtime
        NEXORA::BuildOptions
)

add_test(
    NAME NEXORA.Runtime.Errors
    COMMAND nexora_error_tests
)
add_executable(nexora_result_tests
    tests/result_tests.cpp
)

target_link_libraries(nexora_result_tests
    PRIVATE
        NEXORA::Runtime
        NEXORA::BuildOptions
)

add_test(
    NAME NEXORA.Runtime.Result
    COMMAND nexora_result_tests
)
add_executable(nexora_exception_tests
    tests/exception_tests.cpp
)

target_link_libraries(nexora_exception_tests
    PRIVATE
        NEXORA::Runtime
        NEXORA::BuildOptions
)

add_test(
    NAME NEXORA.Runtime.Exceptions
    COMMAND nexora_exception_tests
)
endif()