#include "cone.h"
std::vector<geometry::Intersection> geometry::Cone::LocalIntersect(
    const commontypes::Ray& ray) const {
    // TODO: refactor this logic out as it's duplicated

    // see pg. 189
    const double a =
        pow(ray.direction().x(), 2) - pow(ray.direction().y(), 2) + pow(ray.direction().z(), 2);

    const double b = 2 * ray.origin().x() * ray.direction().x() -
                     2 * ray.origin().y() * ray.direction().y() +
                     2 * ray.origin().z() * ray.direction().z();

    const double c =
        pow(ray.origin().x(), 2) - pow(ray.origin().y(), 2) + pow(ray.origin().z(), 2);

    // Ray parallel to one of the Cone's halves.
    // Ray may intersect the other half of the Cone (Ray only misses when a & b both == 0)
    if (utility::NearEquals(a, 0.0)) {
        // both miss
        if (utility::NearEquals(b, 0.0)) {
            return {};
        }

        // otherwise a == 0 , b != 0
        // we have a single point of intersection, where t is the following
        const double t = -c / (2 * b);
        std::vector<geometry::Intersection> xs{
            geometry::Intersection{t, std::make_shared<Cone>(*this)}};

        this->IntersectCaps(ray, xs);
        return xs;
    }

    // otherwise, a != 0, so the approach is as it was for the Cylinder intersections.
    const double discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        return {};
    }

    double t0 = (-b - sqrt(discriminant)) / (2 * a);
    double t1 = (-b + sqrt(discriminant)) / (2 * a);
    if (t0 > t1) {
        std::swap(t0, t1);
    }

    const auto this_ptr = std::make_shared<Cone>(*this);
    std::vector<geometry::Intersection> xs{};

    const auto y0 = ray.origin().y() + t0 * ray.direction().y();
    if (IsYBetweenMinMax(y0)) {
        xs.push_back(std::move(geometry::Intersection{t0, this_ptr}));
    }

    const auto y1 = ray.origin().y() + t1 * ray.direction().y();
    if (IsYBetweenMinMax(y1)) {
        xs.push_back(std::move(geometry::Intersection{t1, this_ptr}));
    }

    this->IntersectCaps(ray, xs);
    return xs;
}

commontypes::Vector geometry::Cone::LocalNormalAt(const commontypes::Point& local_point) const {
    // see pg. 190
    // y  = sqrt(point,x^2 + point.z^2)
    double y = sqrt(pow(local_point.x(), 2) + pow(local_point.z(), 2));
    if (local_point.y() > 0) {
        y = -y;
    }

    // as in Cylinder
    if (y < 1) {
        if (local_point.y() >= this->maximum_ - utility::EPSILON_) {
            return commontypes::Vector{0, 1, 0};
        }

        if (local_point.y() <= this->minimum_ + utility::EPSILON_) {
            return commontypes::Vector{0, -1, 0};
        }
    }

    return commontypes::Vector{local_point.x(), y, local_point.z()};
}

bool geometry::Cone::CheckCap(const commontypes::Ray& ray,
                              const double t,
                              PlaneYCoord plane_y_coord) const {
    double radius{};
    if (plane_y_coord == kUseMaximum) {
        radius = fabs(this->maximum_);
    } else if (plane_y_coord == kUseMinimum) {
        radius = fabs(this->minimum_);
    }

    const double x = ray.origin().x() + t * ray.direction().x();
    const double z = ray.origin().z() + t * ray.direction().z();

    return (pow(x, 2) + pow(z, 2)) <= radius + utility::EPSILON_;
}

void geometry::Cone::IntersectCaps(const commontypes::Ray& ray,
                                   std::vector<geometry::Intersection>& xs) const {
    if (!this->IsCapped() || utility::NearEquals(ray.direction().y(), 0.0)) {
        return;
    }

    // Cone's radius at a given y is the absolute value of that y.
    // this differs from Cylinders, as Cylinders have the same radius everywhere
    const double t_min = (this->minimum_ - ray.origin().y()) / ray.direction().y();
    if (geometry::Cone::CheckCap(ray, t_min, kUseMinimum)) {
        xs.emplace_back(t_min, std::make_shared<geometry::Cone>(*this));
    }

    const double t_max = (this->maximum_ - ray.origin().y()) / ray.direction().y();
    if (geometry::Cone::CheckCap(ray, t_max, kUseMaximum)) {
        xs.emplace_back(t_max, std::make_shared<geometry::Cone>(*this));
    }
}
