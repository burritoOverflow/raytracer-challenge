add_subdirectory(common_types)
add_subdirectory(canvas)
add_subdirectory(geometry)
add_subdirectory(lighting)
add_subdirectory(scene)
add_subdirectory(pattern)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Common Canvas Geometry Lighting Scene)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
