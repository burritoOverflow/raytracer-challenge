## RayTracer Challenge

Implementation of [RayTracer Challenge](https://pragprog.com/titles/jbtracer/the-ray-tracer-challenge/) in C++.

Build (via CMake Preset)

```bash
# configure
cmake --preset "Default Debug"

# build
cmake --build --preset "Default Debug"

# run tests
cmake --build --preset "Default Debug" --target test
# or run the test executable directly
./build_debug/test/bin/TestSuite
```

Flags used:

`renderTarget` is required and must be one of the specified options.
bash

```
-width (Camera Width for the render) type: int32 default: 750
-height (Camera Height for the render) type: int32 default: 900
-renderTarget (Name of the render example (chapter10, chapter6, chapter7, patternroomcylinder, patternroomsphere)) type: string default: ""
```
