add_library(Common)

target_sources(Common
        PRIVATE
        src/tuple.cpp
        src/point.cpp
        src/vector.cpp
        src/ray.cpp
        src/matrix.cpp
        src/color.cpp
)

target_include_directories(Common PUBLIC include)
