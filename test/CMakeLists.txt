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

target_sources(${This} PRIVATE unit_tests.cpp)

add_subdirectory(common_types_test)
add_subdirectory(canvas_test)

target_link_libraries(${This} PUBLIC gtest_main Common Canvas)

gtest_discover_tests(${This})
