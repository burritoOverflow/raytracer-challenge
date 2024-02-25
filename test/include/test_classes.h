#ifndef TEST_CLASSES_H
#define TEST_CLASSES_H

#include "pattern.h"
#include "shape.h"

namespace geometry {
class TestShape : public Shape {
   public:
    TestShape() : Shape(), saved_ray_{commontypes::Point{}, commontypes::Vector{}} {};

    inline std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) const override {
        saved_ray_ = ray;  // for the purposes of the tests--from pg. 119-120
        return {};
    }

    inline commontypes::Vector LocalNormalAt(
        const commontypes::Point& local_point) const override {
        // per pg. 121
        return commontypes::Vector{local_point.x(), local_point.y(), local_point.z()};
    }

    // see pg. 119-120
    mutable commontypes::Ray
        saved_ray_;  // we actually do want to mutate this to verify the behavior of `Shape`
};

}  // namespace geometry

namespace pattern {
class TestPattern : public Pattern {
   public:
    TestPattern() : Pattern() {}

    inline commontypes::Color PatternAt(const commontypes::Point& point) const override {
        return commontypes::Color{point.x(), point.y(), point.z()};
    }
};
}  // namespace pattern

#endif  // TEST_CLASSES_H
