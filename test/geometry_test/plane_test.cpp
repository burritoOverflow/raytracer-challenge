#include "plane.h"
#include <gtest/gtest.h>

TEST(PlaneTest, TestNormalOfPlaneIsConstantEverywhere) {
    geometry::Plane p{};
    const auto n1 = p.LocalNormalAt(commontypes::Point{0, 0, 0});
    const auto n2 = p.LocalNormalAt(commontypes::Point{10, 0, -10});
    const auto n3 = p.LocalNormalAt(commontypes::Point{-5, 0, 150});
    const commontypes::Vector expected{0, 1, 0};
    ASSERT_TRUE(n1 == expected);
    ASSERT_TRUE(n2 == expected);
    ASSERT_TRUE(n3 == expected);
}

TEST(PlaneTest, TestIntersectWithRayParallelToPlane) {
    geometry::Plane p{};
    const commontypes::Ray r{commontypes::Point{0, 10, 0}, {0, 0, 1}};
    const auto xs = p.LocalIntersect(r);
    ASSERT_TRUE(xs.empty());
}

TEST(PlaneTest, TestIntersectCoplanarRay) {
    // ray's origin is on the plane; ray's direction is parallel to the plane
    // ray misses in this case (see 122)
    geometry::Plane p{};
    const commontypes::Ray r{commontypes::Point{0, 0, 0}, {0, 0, 1}};
    const auto xs = p.LocalIntersect(r);
    ASSERT_TRUE(xs.empty());
}

TEST(PlaneTest, TestRayIntersectingPlaneFromAbove) {
    geometry::Plane p{};
    const commontypes::Ray r{commontypes::Point{0, 1, 0}, {0, -1, 0}};
    const auto xs = p.LocalIntersect(r);
    ASSERT_TRUE(xs.size() == 1);
    ASSERT_DOUBLE_EQ(xs.at(0).t_, 1.0);
    ASSERT_TRUE((*xs.at(0).object_) == p);
}

TEST(PlaneTest, TestRayIntersectingPlaneFromBelow) {
    geometry::Plane p{};
    commontypes::Ray r{commontypes::Point{0, -1, 0}, {0, 1, 0}};
    const auto xs = p.LocalIntersect(r);
    ASSERT_TRUE(xs.size() == 1);
    ASSERT_DOUBLE_EQ(xs.at(0).t_, 1.0);
    ASSERT_TRUE((*xs.at(0).object_) == p);
}
