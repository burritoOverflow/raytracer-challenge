#include "triangle.h"

commontypes::Vector geometry::Triangle::LocalNormalAt(
    const commontypes::Point& local_point) const {
    return this->normal_;
}

// see: Moller-Trumbore intersection algorithm (pg. 209)
std::vector<geometry::Intersection> geometry::Triangle::LocalIntersect(
    const commontypes::Ray& ray) const {
    const commontypes::Vector dir_cross_e2 = ray.direction().Cross(e2_);
    const double determinant = e1_.Dot(dir_cross_e2);

    // if result is near zero the Ray is parallel and thus misses the Triangle
    if (std::abs(determinant) < utility::EPSILON_) {
        return {};
    }

    const double f = 1.0 / determinant;
    const commontypes::Vector p1_to_origin = commontypes::Vector{ray.origin() - p1_};
    const double u = f * p1_to_origin.Dot(dir_cross_e2);

    // Ray misses if u is not between 0-1
    if (u < 0 || u > 1) {
        return {};
    }

    // check cases: if Ray misses p1-p2 edge and ray misses p2-p3 edge
    // see pg. 211
    const commontypes::Vector origin_cross_e1 = p1_to_origin.Cross(e1_);
    const double v = f * ray.direction().Dot(origin_cross_e1);

    if (v < 0 || (u + v) > 1) {
        return {};
    }

    // case where there exists an Intersection
    const double t = f * e2_.Dot(origin_cross_e1);
    return {geometry::Intersection{t, std::make_shared<geometry::Triangle>(*this)}};
}
