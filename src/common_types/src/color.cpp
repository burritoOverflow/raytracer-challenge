#include "color.h"

using namespace commontypes;

// referred to as the Hadamard Product on pg. 18
Color operator*(Color& c1, Color& c2) {
    return {
        c1.Red() * c2.Red(),
        c1.Green() * c2.Green(),
        c1.Blue() * c2.Blue(),
    };
}
