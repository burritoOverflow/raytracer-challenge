#include "cylinder.h"

std::vector<geometry::Intersection> geometry::Cylinder::LocalIntersect(
    const commontypes::Ray& ray) {
    // compute the discriminant
    const double a = pow(ray.direction().x(), 2) + pow(ray.direction().z(), 2);

    // ray parallel to Y axis
    if (utility::NearEquals(a, 0.0)) {
        return {};
    }

    const double b =
        2 * ray.origin().x() * ray.direction().x() + 2 * ray.origin().z() * ray.direction().z();

    const double c = pow(ray.origin().x(), 2) + pow(ray.origin().z(), 2) - 1;

    const double discriminant = pow(b, 2) - 4 * a * c;

    if (discriminant < 0) {
        return {};
    }

    double t0 = (-b - sqrt(discriminant)) / (2 * a);
    double t1 = (-b + sqrt(discriminant)) / (2 * a);

    // order by ascending
    if (t0 > t1) {
        std::swap(t0, t1);
    }

    std::vector<geometry::Intersection> xs{};
    const auto this_ptr = std::make_shared<Cylinder>(*this);

    // compute the y-coordinate at each point of intersection; valid if between min-max
    // add to intersections if between these bounds
    const auto y0 = ray.origin().y() + t0 * ray.direction().y();
    if (this->minimum_ < y0 && y0 < this->maximum_) {
        xs.push_back(std::move(geometry::Intersection{t0, this_ptr}));
    }

    const auto y1 = ray.origin().y() + t1 * ray.direction().y();
    if (this->minimum_ < y1 && y1 < this->maximum_) {
        xs.push_back(std::move(geometry::Intersection{t1, this_ptr}));
    }

    return xs;
}

commontypes::Vector geometry::Cylinder::LocalNormalAt(const commontypes::Point& local_point) {
    // remove the y component (pg. 181)
    return {local_point.x(), 0, local_point.z()};
}
