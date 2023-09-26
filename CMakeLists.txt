cmake_minimum_required(VERSION 3.24)

project(RayTracerChallengeBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(GoogleTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(test)

# strip release binary
set_target_properties(RayTracerChallengeBook PROPERTIES LINK_FLAGS_RELEASE -s)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
