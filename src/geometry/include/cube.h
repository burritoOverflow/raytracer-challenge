#ifndef CUBE_H
#define CUBE_H

#include "ray.h"
#include "shape.h"

namespace geometry {
// AABB - sides are all aligned with the scene's axis
class Cube : public Shape {
   public:
    Cube() : Shape() {}

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) const override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) const override;

   private:
    std::tuple<double, double> CheckAxis(double origin, double direction) const;
};
}  // namespace geometry

#endif  // CUBE_H
