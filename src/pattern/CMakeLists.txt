add_library(Pattern)

target_sources(Pattern
        PRIVATE
        src/stripepattern.cpp
        src/gradientpattern.cpp
        src/ringpattern.cpp
        src/pattern.cpp
) 

target_include_directories(Pattern PUBLIC include)

target_link_libraries(Pattern PRIVATE Common)