add_library(Geometry
        include/cube.h)

target_sources(Geometry
        PRIVATE
        src/shape.cpp
        src/sphere.cpp
        src/plane.cpp
        src/cube.cpp
        src/cylinder.cpp
        src/cone.cpp
        src/intersection.cpp
)

target_include_directories(Geometry PUBLIC include)

target_link_libraries(Geometry PRIVATE Common Lighting)
