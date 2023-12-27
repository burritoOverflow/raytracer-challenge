#include "cube.h"
#include <algorithm>
#include <memory>
#include "utility.h"

std::vector<geometry::Intersection> geometry::Cube::LocalIntersect(const commontypes::Ray& ray) {
    // min and max for each axis of the Cube
    const auto [xtmin, xtmax] = this->CheckAxis(ray.origin().x(), ray.direction().x());
    const auto [ytmin, ytmax] = this->CheckAxis(ray.origin().y(), ray.direction().y());
    const auto [ztmin, ztmax] = this->CheckAxis(ray.origin().z(), ray.direction().z());

    // find the largest of all min t values and smallest of all max t values
    // intersection is always these two points
    const double tmin = std::fmax(xtmin, std::fmax(ytmin, ztmin));
    const double tmax = std::fmin(xtmax, std::fmin(ytmax, ztmax));

    // if minimum t is further from the origin than maximum t, Ray misses the sphere
    // min_t further from origin (see pg. 173)
    if (tmin > tmax)
        return {};

    return std::vector<geometry::Intersection>{
        geometry::Intersection{
            tmin,
            std::make_shared<geometry::Cube>(*this),
        },
        geometry::Intersection{tmax, std::make_shared<geometry::Cube>(*this)}};
}

// find the actual points of intersection (see pg. 171)
// invoked for each plane in the Cube, this method generalizes
// the Plane LocalIntersect method generalized for Planes offset from the origin
std::tuple<double, double> geometry::Cube::CheckAxis(const double origin, const double direction) {
    // each pair of planes is offset from the origin
    const double tmin_numerator = (-1 - origin);
    const double tmax_numerator = (1 - origin);

    double tmin, tmax;

    if (std::fabs(direction) >= utility::EPSILON_) {
        tmin = tmin_numerator / direction;
        tmax = tmax_numerator / direction;
    } else {
        tmin = tmin_numerator * INFINITY;
        tmax = tmax_numerator * INFINITY;
    }

    if (tmin > tmax) {
        return {tmax, tmin};
    }

    return {tmin, tmax};
}

// each face of the Cube is a Plane with its own normal; normal will be same at every point on the
// face
commontypes::Vector geometry::Cube::LocalNormalAt(const commontypes::Point& local_point) {
    const double abs_x = abs(local_point.x());
    const double abs_y = abs(local_point.y());
    const double abs_z = abs(local_point.z());

    // component with the largest absolute value (face is always the one matching the component
    // whose absolute value is 1); can't trust floating-point equality, however it's also true that
    // the largest (absolute) value component is the Normal
    const double max_component = std::max({abs_x, abs_y, abs_z});

    if (max_component == abs_x) {
        return commontypes::Vector{local_point.x(), 0, 0};
    } else if (max_component == abs_y) {
        return commontypes::Vector{0, local_point.y(), 0};
    }

    return commontypes::Vector{0, 0, local_point.z()};
}
