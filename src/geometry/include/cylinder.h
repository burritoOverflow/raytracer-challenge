#ifndef CYLINDER_H
#define CYLINDER_H

#include "intersection.h"
#include "shape.h"

namespace geometry {
class Cylinder : public Shape {
   public:
    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) override;
};
}  // namespace geometry

#endif  // CYLINDER_H
