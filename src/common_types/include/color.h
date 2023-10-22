#ifndef COLOR_H
#define COLOR_H

#include "tuple.h"

namespace commontypes {
class Color final : public Tuple {
   public:
    Color() : Tuple() {}
    Color(const double r, const double g, const double b) : Tuple(r, g, b, 0.0) {}
    explicit Color(const Tuple& t) : Tuple(t.x(), t.y(), t.z(), 0.0) {}

    // redundant as "black" is the same as the default ctor, but nice for reading
    static Color Black() { return {0, 0, 0}; }

    inline double Red() const { return e_[0]; }
    inline double Green() const { return e_[1]; }
    inline double Blue() const { return e_[2]; }
};
}  // namespace commontypes

commontypes::Color operator*(const commontypes::Color& c1, const commontypes::Color& c2);

#endif  // COLOR_H
