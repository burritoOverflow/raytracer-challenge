#ifndef RAY_H
#define RAY_H

#include "matrix.h"
#include "point.h"
#include "vector.h"

namespace commontypes {
class Ray {
   public:
    Ray(const Point& origin, const Vector& direction) : origin_(origin), direction_(direction) {}
    Point origin() const { return origin_; }
    Vector direction() const { return direction_; }

    Point Position(const double t) { return Point(origin_ + direction_ * t); }

    // applies the transformation Matrix to the Ray, returning a new Ray with a transformed origin
    // and direction; new Ray is returned as the original is used to calculate locations in World
    // space
    Ray Transform(const Matrix& m) const;

   private:
    Point origin_;
    Vector direction_;
};
}  // namespace commontypes

commontypes::Ray operator*(const commontypes::Ray& r, const commontypes::Matrix& m);

#endif  // RAY_H
