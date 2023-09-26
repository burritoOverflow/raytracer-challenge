#ifndef POINT_H
#define POINT_H

#include "tuple.h"

namespace commontypes {
class Point final : public Tuple {
   public:
    Point() : Tuple() {}
    Point(const double x, const double y, const double z) : Tuple(x, y, z, 1.0) {}
};
}  // namespace commontypes

#endif  // POINT_H
