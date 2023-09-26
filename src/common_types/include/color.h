#ifndef COLOR_H
#define COLOR_H

#include "tuple.h"

namespace commontypes {
class Color final : public Tuple {
   public:
    Color() : Tuple() {}
    Color(const double r, const double g, const double b) : Tuple(r, g, b, 0.0) {}

    inline double Red() const { return e_[0]; }
    inline double Green() const { return e_[1]; }
    inline double Blue() const { return e_[2]; }
};
}  // namespace commontypes

commontypes::Color operator*(commontypes::Color& c1, commontypes::Color& c2);

#endif  // COLOR_H
