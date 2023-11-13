#ifndef PLANE_H
#define PLANE_H

#include <vector>
#include "intersection.h"
#include "shape.h"

namespace geometry {
// flat surface extends infinitely in two dimensions, ours is xz
class Plane : public Shape {
   public:
    Plane() : Shape() {}

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) override;
};
}  // namespace geometry

#endif  // PLANE_H
