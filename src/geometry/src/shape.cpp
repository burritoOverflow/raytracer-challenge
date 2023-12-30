#include "shape.h"

uint64_t geometry::Shape::SHAPE_ID = 0;

std::vector<geometry::Intersection> geometry::Shape::Intersect(const commontypes::Ray& ray) {
    // transforms the Ray and calls the Shape's `LocalIntersect` w/ the transformed Ray
    const commontypes::Ray transformed_ray = ray.Transform(transform_.Inverse());
    return LocalIntersect(transformed_ray);
}

commontypes::Vector geometry::Shape::NormalAt(const commontypes::Point& point) {
    // first, convert the ray to object space
    const commontypes::Point local_point = commontypes::Point(transform_.Inverse() * point);

    // as above, first convert the point to object space (see discussion on pg. 118)
    const commontypes::Vector local_normal = LocalNormalAt(local_point);
    commontypes::Vector world_normal =
        commontypes::Vector{this->transform_.Inverse().Transpose() * local_normal};

    world_normal.e_[3] = 0.0;
    return commontypes::Vector{world_normal.Normalize()};
}

bool operator==(const geometry::Shape& s1, const geometry::Shape& s2) {
    return s1.id() == s2.id();
}
