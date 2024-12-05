add_subdirectory(common_types)
add_subdirectory(canvas)
add_subdirectory(geometry)
add_subdirectory(lighting)
add_subdirectory(scene)
add_subdirectory(pattern)

include(FetchContent)

FetchContent_Declare(
    gflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    GIT_TAG v2.2.2
)

# avoid building the tests for GFlags used by CTest
set(BUILD_TESTING OFF)

# if not explicitly disabled, the static library will be built alongside the shared library
# and the executable will be linked with the static lib, despite the presence of the static library
# and the intent to use the shared library
if(DEFINED BUILD_SHARED_LIBS)
    set(BUILD_STATIC_LIBS OFF)
endif()

FetchContent_MakeAvailable(gflags)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE gflags Common Canvas Geometry Lighting Scene Pattern)
