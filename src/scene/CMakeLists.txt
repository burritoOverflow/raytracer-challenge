add_library(Scene)

target_sources(Scene
        PRIVATE
        src/world.cpp
        src/camera.cpp)

target_include_directories(Scene PUBLIC include)

target_link_libraries(Scene PRIVATE Common Lighting Canvas Geometry)
