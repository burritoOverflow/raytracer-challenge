set(This TestSuite)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)

mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

add_executable(${This} "")

# TestSuite binary in the `test/bin` subdirectory
set_target_properties(${This}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/bin"
        )

target_sources(${This} PRIVATE unit_tests.cpp)

add_subdirectory(common_types_test)
add_subdirectory(canvas_test)
add_subdirectory(geometry_test)
add_subdirectory(lighting_test)
add_subdirectory(scene_test)
add_subdirectory(pattern_test)

target_include_directories(${This} PRIVATE include)

target_link_libraries(${This} PUBLIC gtest_main Common Canvas Geometry Lighting Scene Pattern)

gtest_discover_tests(${This})
