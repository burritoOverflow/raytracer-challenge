#include "shape.h"

uint64_t geometry::Shape::SHAPE_ID = 0;

std::vector<geometry::Intersection> geometry::Shape::Intersect(const commontypes::Ray& ray) const {
    // transforms the Ray and calls the Shape's `LocalIntersect` w/ the transformed Ray
    const commontypes::Ray transformed_ray = ray.Transform(transform_.Inverse());
    return LocalIntersect(transformed_ray);
}

commontypes::Vector geometry::Shape::NormalAt(const commontypes::Point& world_point) const {
    // first, convert the ray to object space
    const commontypes::Point local_point = this->WorldToObject(world_point);

    // as above, first convert the point to object space (see discussion on pg. 118)
    const commontypes::Vector local_normal = LocalNormalAt(local_point);

    // revised implementation on pg. 200
    return this->NormalToWorld(local_normal);
}

commontypes::Point geometry::Shape::WorldToObject(const commontypes::Point& point) const {
    commontypes::Point _point = point;

    // if a parent is present, first convert the Point to its parent's Space
    if (this->HasParent()) {
        // account for parents
        _point = this->parent_->WorldToObject(point);
    }
    return commontypes::Point{this->transform_.Inverse() * _point};
}

commontypes::Vector geometry::Shape::NormalToWorld(const commontypes::Vector& normal) const {
    // approach initially implemented on pg. 79
    commontypes::Vector _normal =
        commontypes::Vector{this->Transform().Inverse().Transpose() * normal};

    // set w = 0
    _normal.e_[3] = 0.0;
    _normal = commontypes::Vector{_normal.Normalize()};

    if (this->HasParent()) {
        // account for parents here as well.
        _normal = commontypes::Vector{this->parent_->NormalToWorld(_normal)};
    }

    return _normal;
}

// shape equality is strictly concerned with the `id` of the Shape as this is guaranteed to be
// unique
bool operator==(const geometry::Shape& s1, const geometry::Shape& s2) {
    return s1.id() == s2.id();
}
