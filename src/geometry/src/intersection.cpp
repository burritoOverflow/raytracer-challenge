#include "intersection.h"
#include <algorithm>
#include "sphere.h"

// returns the hit from a vector of Intersections
std::optional<geometry::Intersection> geometry::Intersection::Hit(
    const std::vector<geometry::Intersection>& xs) {
    // recall that negative t values can be ignored and the intersections are returned in
    // increasing order from the `Intersect` method (i.e sorting should not be necessary)
    // however, the test suite has us using vectors of values constructed not in increasing order,
    // so we'll create a sorted copy of the Intersections
    std::vector<geometry::Intersection> xs_sorted{xs.size()};
    std::partial_sort_copy(xs.begin(), xs.end(), xs_sorted.begin(), xs_sorted.end());

    for (const auto& intersection : xs_sorted) {
        // now return first non-negative from the sorted values
        if (intersection.t_ >= 0)
            return intersection;
    }
    return std::nullopt;
}

geometry::Computations geometry::Intersection::PrepareComputations(commontypes::Ray& r) const {
    geometry::Computations computations{};

    computations.t_ = t_;
    computations.object_ = object_;
    computations.point_ = r.Position(computations.t_);
    computations.eye_vector_ = -r.direction();

    computations.normal_vector_ = (*computations.object_).NormalAt(computations.point_);
    if (computations.normal_vector_.Dot(computations.eye_vector_) < 0) {
        computations.inside_ = true;
        computations.normal_vector_ = -computations.normal_vector_;
    }

    // reflect the ray's direction around the object's normal vector
    computations.reflect_vector_ = r.direction().Reflect(computations.normal_vector_);

    // bump the point a bit in the direction of the normal
    computations.over_point_ =
        commontypes::Point{computations.point_ + computations.normal_vector_ * utility::EPSILON_};

    return computations;
}

bool operator==(const geometry::Intersection& i1, const geometry::Intersection& i2) {
    return i1.object_ == i2.object_ && utility::NearEquals(i1.t_, i2.t_);
}
