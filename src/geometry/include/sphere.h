#ifndef SPHERE_H
#define SPHERE_H

#include <cstdint>
#include <vector>
#include "identitymatrix.h"
#include "intersection.h"
#include "material.h"
#include "point.h"
#include "ray.h"
#include "shape.h"

namespace geometry {
class Sphere : public Shape {
   public:
    // default Point ctor returns a Point with (0,0,0)
    Sphere() : Shape(), radii_(1.0), origin_(commontypes::Point()) {}

    inline commontypes::Point origin() const { return origin_; }

    inline double radii() const { return radii_; }

    commontypes::Vector NormalAt(const commontypes::Point& p) const;

    // "collection of t values where Ray intersects the Sphere" -- we're using vec of Structs
    // containing the t val for an intersection and the id for the Sphere
    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) override;

   private:
    double radii_;  // expectation is that by default these are all unit spheres (see page 59)
    // must be incremented in each ctor, as above (see uniqueness constraint)

    commontypes::Point origin_;  // expectation is that the Sphere is situated at the World's
                                 // origin (0,0,0) (also 59)
};

}  // namespace geometry

bool operator==(const geometry::Sphere& s1, const geometry::Sphere& s2);

#endif  // SPHERE_H
