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

geometry::Computations geometry::Intersection::PrepareComputations(
    commontypes::Ray& r,
    const std::vector<Intersection>& intersections) const {
    geometry::Computations computations{};
    std::vector<std::shared_ptr<geometry::Shape>> containers{};

    for (const auto& intersection : intersections) {
        const bool is_intersection_is_the_hit = intersection == *this;

        if (is_intersection_is_the_hit) {
            if (containers.empty()) {
                // set n1 to 1 if list is empty
                computations.n1 = 1.0;
            } else {
                // otherwise, if intersection is the hit, set n1 to the
                // RefractiveIndex of the last object in the containers
                computations.n1 = containers.back()->Material()->RefractiveIndex();
            }
        }

        const uint64_t shape_id = intersection.object_->id();
        const auto it =
            std::find_if(containers.begin(), containers.end(),
                         [shape_id](const std::shared_ptr<geometry::Shape>& shape_ptr) {
                             return shape_ptr->id() == shape_id;
                         });

        if (it == containers.end()) {
            // intersection is entering the object, add to the list
            containers.push_back(intersection.object_);
        } else {
            // if intersection is already in the list, then this intersection must be exiting the
            // object, so we remove it.
            containers.erase(it);
        }

        if (is_intersection_is_the_hit) {
            if (containers.empty()) {
                // no containing object, as before
                computations.n2 = 1.0;
            } else {
                // RefractiveIndex of the last object in the containers list
                computations.n2 = containers.back()->Material()->RefractiveIndex();
            }
        }
    }

    computations.t_ = t_;
    computations.object_ = object_;
    computations.point_ = r.Position(computations.t_);
    computations.eye_vector_ = commontypes::Vector{-r.direction()};

    computations.normal_vector_ = (*computations.object_).NormalAt(computations.point_);
    if (computations.normal_vector_.Dot(computations.eye_vector_) < 0) {
        computations.inside_ = true;
        computations.normal_vector_ = commontypes::Vector{-computations.normal_vector_};
    }

    // reflect the ray's direction around the object's normal vector
    computations.reflect_vector_ =
        commontypes::Vector{r.direction().Reflect(computations.normal_vector_)};

    // bump the point a bit in the direction of the normal
    computations.over_point_ =
        commontypes::Point{computations.point_ + computations.normal_vector_ * utility::EPSILON_};

    // as above, see pg. 154
    computations.under_point_ =
        commontypes::Point{computations.point_ - computations.normal_vector_ * utility::EPSILON_};

    return computations;
}

double geometry::Schlick(const geometry::Computations& comps) {
    double cos = comps.eye_vector_.Dot(comps.normal_vector_);

    // total internal reflection can only occur if n1 > n2
    if (comps.n1 > comps.n2) {
        const double n = comps.n1 / comps.n2;
        const double sin2_t = pow(n, 2) * (1 - pow(cos, 2));

        if (sin2_t > 1.0)
            return 1.0;

        const double cos_t = sqrt(1.0 - sin2_t);

        // with n1 > n2, use cos(theta_t) instead
        cos = cos_t;
    }
    const double r0 = pow(((comps.n1 - comps.n2) / (comps.n1 + comps.n2)), 2);
    return r0 + (1 - r0) * pow((1 - cos), 5);
}

bool operator==(const geometry::Intersection& i1, const geometry::Intersection& i2) {
    return i1.object_ == i2.object_ && utility::NearEquals(i1.t_, i2.t_);
}
