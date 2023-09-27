add_library(Canvas)

target_sources(Canvas
        PRIVATE
        src/canvas.cpp
)

target_include_directories(Canvas PUBLIC include)

target_link_libraries(Canvas PRIVATE Common)