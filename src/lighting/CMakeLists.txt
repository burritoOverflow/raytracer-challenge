add_library(Lighting)

target_sources(Lighting
        PRIVATE
        src/pointlight.cpp
        src/lighting.cpp
        src/material.cpp
        )

target_include_directories(Lighting PUBLIC include)

target_link_libraries(Lighting PRIVATE Common Pattern)
