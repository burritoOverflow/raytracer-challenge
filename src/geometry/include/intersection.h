#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "utility.h"

namespace geometry {
// forward declare the Sphere here as this is a circular dependency
class Sphere;

struct Intersection {
    // the t value where a Ray intersects the Sphere
    double t_;
    std::shared_ptr<Sphere>
        object_;  // the id of the Sphere for which this intersection was located

    static std::optional<Intersection> Hit(const std::vector<Intersection>& xs);

    // this is strictly used for comparison when sorting for retrieving the hit (method above)
    inline bool operator<(const Intersection& rhs) const {
        return std::tie(t_, object_) < std::tie(rhs.t_, rhs.object_);
    }
};
}  // namespace geometry

bool operator==(const geometry::Intersection& i1, const geometry::Intersection& i2);

#endif  // INTERSECTION_H
