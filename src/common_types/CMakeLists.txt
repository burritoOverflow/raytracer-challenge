add_library(Common)

target_sources(Common
    PRIVATE
    src/tuple.cpp
    src/point.cpp
    src/vector.cpp
)

target_include_directories(Common PUBLIC include)
