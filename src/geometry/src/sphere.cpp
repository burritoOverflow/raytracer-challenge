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

commontypes::Vector geometry::Sphere::LocalNormalAt(const commontypes::Point& local_point) {
    return commontypes::Vector{local_point - commontypes::Point{}};
}

bool operator==(const geometry::Sphere& s1, const geometry::Sphere& s2) {
    return utility::NearEquals(s1.radii(), s2.radii()) && s1.origin() == s2.origin();
}
