#include "sphere.h"

// unique identifier for Spheres
uint64_t geometry::Sphere::SPHERE_ID = 0;

std::vector<geometry::Intersection> geometry::Sphere::Intersect(const commontypes::Ray& ray) {
    // ray is transformed by the inverse of the Sphere's transformation Matrix (see pg 69)
    // must be translated before performing calculations
    commontypes::Ray transformed_ray = ray.Transform(transform_.Inverse());

    // vector from Sphere's center to Ray's origin (pg. 62)
    const commontypes::Vector sphere_to_ray = transformed_ray.origin() - origin_;
    const double a = transformed_ray.direction().Dot(transformed_ray.direction());
    const double b = 2 * transformed_ray.direction().Dot(sphere_to_ray);
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

bool operator==(const geometry::Sphere& s1, const geometry::Sphere& s2) {
    if (s1.id() != s2.id())
        return false;

    if (!utility::NearEquals(s1.radii(), s2.radii()))
        return false;

    if (s1.origin() != s2.origin())
        return false;

    return true;
}
