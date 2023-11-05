#include "shape.h"

uint64_t geometry::Shape::SHAPE_ID = 0;

std::vector<geometry::Intersection> geometry::Shape::Intersect(const commontypes::Ray& ray) {
    const commontypes::Ray transformed_ray = ray.Transform(transform_.Inverse());
    return LocalIntersect(transformed_ray);
}
