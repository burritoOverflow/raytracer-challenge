add_subdirectory(common_types)
add_subdirectory(canvas)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Common Canvas)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
