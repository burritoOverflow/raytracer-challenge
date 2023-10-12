#include "sphere.h"

// unique identifier for Spheres
uint64_t geometry::Sphere::SPHERE_ID = 0;

std::vector<geometry::Intersection> geometry::Sphere::Intersect(const commontypes::Ray& ray) {
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
    if (t1 >= t2) {
        return {geometry::Intersection{t2, id_}, geometry::Intersection{t1, id_}};
    } else {
        return {geometry::Intersection{t1, id_}, geometry::Intersection{t2, id_}};
    }
}
