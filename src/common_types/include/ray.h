#ifndef RAY_H
#define RAY_H

#include "point.h"
#include "vector.h"

namespace commontypes {
class Ray {
   public:
    Ray(const Point& origin, const Vector& direction) : origin_(origin), direction_(direction) {}
    Point origin() const { return origin_; }
    Vector direction() const { return direction_; }

    Point Position(const double t) { return Point(origin_ + direction_ * t); }

   private:
    Point origin_;
    Vector direction_;
};
}  // namespace commontypes
#endif  // RAY_H
