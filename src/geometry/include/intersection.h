#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <cstdint>

namespace geometry {
struct Intersection {
    // the t value where a Ray intersects the Sphere
    double t_;
    uint64_t sphere_id_;  // the id of the Sphere for which this intersection was located
};
}  // namespace geometry

#endif  // INTERSECTION_H
