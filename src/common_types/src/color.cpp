#include "color.h"

// referred to as the Hadamard Product on pg. 18
commontypes::Color operator*(const commontypes::Color& c1, const commontypes::Color& c2) {
    return commontypes::Color{
        c1.Red() * c2.Red(),
        c1.Green() * c2.Green(),
        c1.Blue() * c2.Blue(),
    };
}
