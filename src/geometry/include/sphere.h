#ifndef SPHERE_H
#define SPHERE_H

#include <cstdint>
#include <vector>
#include "intersection.h"
#include "point.h"
#include "ray.h"

namespace geometry {
class Sphere {
   public:
    // default Point ctor returns a Point with (0,0,0)
    Sphere() : origin_(commontypes::Point()), radii_(1.0), id_(SPHERE_ID++) {}

    // "collection of t values where Ray intersects the Sphere" -- we're using vec of Structs
    // containing the t val for an intersection and the id for the Sphere
    std::vector<Intersection> Intersect(const commontypes::Ray& ray);

   private:
    uint64_t id_;   // each sphere must have a unique identifier
    double radii_;  // expectation is that by default these are all unit spheres (see page 59)
    commontypes::Point origin_;  // expectation is that the Sphere is situated at the World's
                                 // origin (0,0,0) (also 59)

    // must be incremented in each ctor, as above (see uniqueness constraint)
    static uint64_t SPHERE_ID;
};

}  // namespace geometry

#endif
