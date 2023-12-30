#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "point.h"
#include "ray.h"
#include "vector.h"

namespace geometry {
class Shape;

// encapsulates some computations related to the intersection
struct Computations {
    std::shared_ptr<Shape> object_;
    double t_;
    commontypes::Point point_;
    commontypes::Point
        over_point_;  // for avoiding self-shadowing (adjust the point slightly in the direction of
                      // the normal, just before testing for shadows (see pg. 114)
    commontypes::Point under_point_;  // lies just beneath surface
    commontypes::Vector eye_vector_;
    commontypes::Vector normal_vector_;
    commontypes::Vector reflect_vector_;
    bool inside_{false};  // true if hit occurs inside the object

    // n1 and n2 are the refractive indices of materials on either side of the ray-object
    // intersection
    // with n1 belonging to the material being exited and n2 belonging to the material being
    // entered
    double n1;
    double n2;
};

class Intersection {
   public:
    Intersection() : t_(0), object_(nullptr) {}

    explicit Intersection(const double t, const std::shared_ptr<Shape>& object_ptr)
        : t_(t), object_(object_ptr) {}

    static std::optional<Intersection> Hit(const std::vector<Intersection>& xs);

    // see details of the addition and purpose of the `intersections` parameter on pg. 153
    Computations PrepareComputations(
        commontypes::Ray& r,
        const std::vector<Intersection>& intersections = std::vector<Intersection>()) const;

    // this is strictly used for comparison when sorting for retrieving the hit (method above)
    inline bool operator<(const Intersection& rhs) const {
        return std::tie(t_, object_) < std::tie(rhs.t_, rhs.object_);
    }

    // the t value where a Ray intersects the Shape
    double t_;

    std::shared_ptr<Shape> object_;  //  the Shape for which this intersection was located
};

double Schlick(const Computations& comps);
}  // namespace geometry

bool operator==(const geometry::Intersection& i1, const geometry::Intersection& i2);

#endif  // INTERSECTION_H
