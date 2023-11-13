#include "plane.h"
#include "utility.h"

std::vector<geometry::Intersection> geometry::Plane::LocalIntersect(const commontypes::Ray& ray) {
    // plane is in xz, it has no slope in y at all.
    // if the ray's direction vector has no slope in y, it is parallel to the plane
    if (std::abs(ray.direction().y()) < utility::EPSILON_) {
        return {};
    }

    // NOTE - this calculation is only appropriate for xz planes, as this example is
    const auto t = -ray.origin().y() / ray.direction().y();
    const auto intersection = geometry::Intersection{t, std::make_shared<geometry::Plane>(*this)};
    return {intersection};
}

commontypes::Vector geometry::Plane::LocalNormalAt(const commontypes::Point& local_point) {
    // with no curvature, the normal is constant everywhere
    return {0, 1, 0};
}
