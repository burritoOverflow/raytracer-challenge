#include "shape.h"

uint64_t geometry::Shape::SHAPE_ID = 0;

std::vector<geometry::Intersection> geometry::Shape::Intersect(const commontypes::Ray& ray) {
    // transforms the Ray and calls the Shape's `LocalIntersect` w/ the transformed Ray
    const commontypes::Ray transformed_ray = ray.Transform(transform_.Inverse());
    return LocalIntersect(transformed_ray);
}

commontypes::Vector geometry::Shape::NormalAt(const commontypes::Point& point) {
    const commontypes::Point local_point = commontypes::Point(transform_.Inverse() * point);
    const commontypes::Vector local_normal = LocalNormalAt(local_point);
    commontypes::Vector world_normal = this->transform_.Inverse().Transpose() * local_normal;

    world_normal.e_[3] = 0.0;
    return commontypes::Vector{world_normal.Normalize()};
}
