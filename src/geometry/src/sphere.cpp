#include "sphere.h"

std::vector<geometry::Intersection> geometry::Sphere::LocalIntersect(
    const commontypes::Ray& ray) const {
    // vector from Sphere's center to Ray's origin (pg. 62)
    const commontypes::Vector sphere_to_ray = commontypes::Vector{ray.origin() - origin_};

    const double a = ray.direction().Dot(ray.direction());
    const double b = 2 * ray.direction().Dot(sphere_to_ray);
    const double c = sphere_to_ray.Dot(sphere_to_ray) - 1;
    const double discriminant = pow(b, 2) - 4 * a * c;

    // Ray misses the Sphere; no intersections occur
    if (discriminant < 0) {
        return {};
    }

    // when both t values are the same, we've encountered a case where a Ray
    // hits a Sphere at a tangent
    double t1 = (-b - sqrt(discriminant)) / (2 * a);
    double t2 = (-b + sqrt(discriminant)) / (2 * a);

    std::shared_ptr<Sphere> object = std::make_shared<Sphere>(*this);

    // return t values in increasing order
    if (t1 > t2) {
        std::swap(t1, t2);
    }

    return {geometry::Intersection{t1, object}, geometry::Intersection{t2, std::move(object)}};
}

commontypes::Vector geometry::Sphere::LocalNormalAt(const commontypes::Point& local_point) const {
    return commontypes::Vector{local_point - commontypes::Point{}};
}

bool operator==(const geometry::Sphere& s1, const geometry::Sphere& s2) {
    return utility::NearEquals(s1.radii(), s2.radii()) && s1.origin() == s2.origin();
}
