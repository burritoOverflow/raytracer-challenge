add_library(Geometry)

target_sources(Geometry
        PRIVATE
        src/sphere.cpp
        src/intersection.cpp
)

target_include_directories(Geometry PUBLIC include)

target_link_libraries(Geometry PRIVATE Common Lighting)
