#ifndef CUBE_H
#define CUBE_H

#include "ray.h"
#include "shape.h"

namespace geometry {
class Cube : public Shape {
   public:
    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) override;

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) override;

   private:
    std::tuple<double, double> CheckAxis(double origin, double direction);
};
}  // namespace geometry

#endif  // CUBE_H
