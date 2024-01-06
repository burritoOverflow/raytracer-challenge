add_library(Common)

target_sources(Common
        PRIVATE
        src/tuple.cpp
        src/ray.cpp
        src/matrix.cpp
        src/viewtransform.cpp
        src/color.cpp
        src/utility.cpp
)

target_include_directories(Common PUBLIC include)
