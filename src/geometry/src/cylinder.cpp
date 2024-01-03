#include "cylinder.h"

std::vector<geometry::Intersection> geometry::Cylinder::LocalIntersect(
    const commontypes::Ray& ray) {
    // compute the discriminant
    const double a = pow(ray.direction().x(), 2) + pow(ray.direction().z(), 2);

    std::vector<geometry::Intersection> xs{};

    // ray parallel to Y axis; skip the Cylinder intersection logic in this case
    if (!utility::NearEquals(a, 0.0)) {
        const double b = 2 * ray.origin().x() * ray.direction().x() +
                         2 * ray.origin().z() * ray.direction().z();

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
    }

    this->IntersectCaps(ray, xs);
    return xs;
}

commontypes::Vector geometry::Cylinder::LocalNormalAt(const commontypes::Point& local_point) {
    // the square of the distance from the y-axis
    const double dist = pow(local_point.x(), 2) + pow(local_point.z(), 2);

    // end caps are Planes, so the normal is the same at every point
    if (dist < 1) {
        // either of these conditions indicate the normal must be on one of the
        // end caps (see pg. 187-188)
        if (local_point.y() >= this->maximum_ - utility::EPSILON_) {
            return commontypes::Vector{0, 1, 0};
        }

        if (local_point.y() <= this->minimum_ + utility::EPSILON_) {
            return commontypes::Vector{0, -1, 0};
        }
    }

    // otherwise Cylinder, remove the y component (pg. 181)
    return commontypes::Vector{local_point.x(), 0, local_point.z()};
}

// check to see if the intersection at `t` is within a radius of 1 from the y-axis
bool geometry::Cylinder::CheckCap(const commontypes::Ray& ray, const double t) {
    const double x = ray.origin().x() + t * ray.direction().x();
    const double z = ray.origin().z() + t * ray.direction().z();
    return (pow(x, 2) + pow(z, 2)) <= 1;
}

// pg. 186
void geometry::Cylinder::IntersectCaps(const commontypes::Ray& ray,
                                       std::vector<geometry::Intersection>& xs) {
    if (!this->IsCapped() || utility::NearEquals(ray.direction().y(), 0.0)) {
        return;
    }

    // check for intersection with lower end cap by intersecting the Ray with the Plane
    // at y = cylinder.min
    const double t_min = (this->minimum_ - ray.origin().y()) / ray.direction().y();
    if (geometry::Cylinder::CheckCap(ray, t_min)) {
        xs.emplace_back(t_min, std::make_shared<geometry::Cylinder>(*this));
    }

    // as above, but for upper end cap by intersecting the Ray w/ Plane at y = cylinder.maximum
    const double t_max = (this->maximum_ - ray.origin().y()) / ray.direction().y();
    if (geometry::Cylinder::CheckCap(ray, t_max)) {
        xs.emplace_back(t_max, std::make_shared<geometry::Cylinder>(*this));
    }
}
