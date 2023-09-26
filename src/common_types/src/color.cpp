#include "color.h"

using namespace commontypes;

// referred to as the Hadamard Product on pg. 18
Color operator*(commontypes::Color& c1, commontypes::Color& c2) {
    return Color{
        c1.Red() * c2.Red(),
        c1.Green() * c2.Green(),
        c1.Blue() * c2.Blue(),
    };
}
