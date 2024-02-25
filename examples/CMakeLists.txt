set(RAYTRACER_EXECUTABLES "${PROJECT_NAME}-Chapter06RenderExample" "${PROJECT_NAME}-PatternRoomRefractiveCylinder" "${PROJECT_NAME}-Chapter7SceneRender" "${PROJECT_NAME}-Chapter10PlaneRender" "${PROJECT_NAME}-PatternRoomRefractiveSphere" "${PROJECT_NAME}-Chapter14GroupHexagonExample")
set(RAYTRACER_SOURCES chapter06renderexample.cpp patternroomrefractivecylinder.cpp chapter7scene.cpp chapter10planerender.cpp patternroomrefractivesphere.cpp chapter14hexagonworld.cpp)

foreach (src target IN ZIP_LISTS RAYTRACER_SOURCES RAYTRACER_EXECUTABLES)
    message("Found Target: ${target} for source: ${src}")

    add_executable(${target} ""
            include/util.h)

    target_include_directories(${target} PRIVATE include)

    target_sources(${target} PRIVATE ${src})

    target_link_libraries(${target} PRIVATE Common Canvas Geometry Lighting Scene Pattern)

    # strip release binaries
    set_target_properties(${target} PROPERTIES LINK_FLAGS_RELEASE -s)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target} PRIVATE -march=native)
    endif ()
endforeach ()

