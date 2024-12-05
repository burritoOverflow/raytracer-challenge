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

FetchContent_MakeAvailable(gflags)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE gflags Common Canvas Geometry Lighting Scene Pattern)
