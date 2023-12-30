#ifndef POINT_H
#define POINT_H

#include <stdexcept>
#include "tuple.h"

namespace commontypes {
class Point final : public Tuple {
   public:
    Point() : Tuple() {}
    explicit Point(const double x, const double y, const double z) : Tuple(x, y, z, 1.0) {}
    explicit Point(const Tuple& t) {
        if (!t.IsPoint())
            throw std::invalid_argument("Provided Tuple is not a point");
        e_[0] = t.e_[0];
        e_[1] = t.e_[1];
        e_[2] = t.e_[2];
        e_[3] = t.e_[3];
    }
};
}  // namespace commontypes

#endif  // POINT_H
