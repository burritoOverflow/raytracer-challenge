#include "sphere.h"

std::vector<geometry::Intersection> geometry::Sphere::LocalIntersect(const commontypes::Ray& ray) {
    // vector from Sphere's center to Ray's origin (pg. 62)
    const commontypes::Vector sphere_to_ray = ray.origin() - origin_;

    const double a = ray.direction().Dot(ray.direction());
    const double b = 2 * ray.direction().Dot(sphere_to_ray);
    const double c = sphere_to_ray.Dot(sphere_to_ray) - 1;
    const double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return {};
    }

    const double t1 = (-b - sqrt(discriminant)) / (2 * a);
    const double t2 = (-b + sqrt(discriminant)) / (2 * a);

    // return t values in increasing order
    std::shared_ptr<Sphere> object = std::make_shared<Sphere>(*this);

    if (t1 >= t2) {
        return {geometry::Intersection{t2, object}, geometry::Intersection{t1, object}};
    } else {
        return {geometry::Intersection{t1, object}, geometry::Intersection{t2, object}};
    }
}

commontypes::Vector geometry::Sphere::NormalAt(const commontypes::Point& p) const {
    // point in object space (from world space)
    const auto object_point = commontypes::Point{this->transform_.Inverse() * p};

    // calculate the normal in object space
    const commontypes::Vector object_normal = object_point - commontypes::Point{};

    // and now the normal in world space
    commontypes::Vector world_normal = this->transform_.Inverse().Transpose() * object_normal;

    // see the book's rationale for doing this explicitly despite being a Vector
    world_normal.e_[3] = 0.0;
    return world_normal.Normalize();
}

bool operator==(const geometry::Sphere& s1, const geometry::Sphere& s2) {
    return utility::NearEquals(s1.radii(), s2.radii()) && s1.origin() == s2.origin();
}
