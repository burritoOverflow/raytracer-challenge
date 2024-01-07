cmake_minimum_required(VERSION 3.24)

project(RayTracerChallengeBook LANGUAGES CXX)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(GoogleTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)

