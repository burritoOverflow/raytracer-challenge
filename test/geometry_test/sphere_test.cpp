#include "sphere.h"
#include <gtest/gtest.h>
#include "ray.h"

TEST(SphereTest, TestRayIntersectsSphereAtTwoPoints) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere s{};
    const std::vector<geometry::Intersection> xs = s.Intersect(r);
    EXPECT_EQ(xs.size(), 2);
    EXPECT_DOUBLE_EQ(xs.at(0).t_, 4.0);
    EXPECT_DOUBLE_EQ(xs.at(1).t_, 6.0);
}

TEST(SphereTest, TestRayIntersectsSphereAtTangent) {
    commontypes::Ray r{commontypes::Point{0, 1, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere s{};
    const std::vector<geometry::Intersection> xs = s.Intersect(r);
    EXPECT_EQ(xs.size(), 2);

    // in this case, there is only one intersection (see rationale on page 60)
    const double expected = 5.0;
    EXPECT_DOUBLE_EQ(xs.at(0).t_, expected);
    EXPECT_DOUBLE_EQ(xs.at(1).t_, expected);
}

TEST(SphereTest, TestRayMissesSphere) {
    commontypes::Ray r{commontypes::Point{0, 2, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere s{};
    const std::vector<geometry::Intersection> xs = s.Intersect(r);
    EXPECT_EQ(xs.size(), 0);
}

TEST(SphereTest, TestRayOriginatesInsideSphere) {
    commontypes::Ray r{commontypes::Point{0, 0, 0}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere s{};
    const std::vector<geometry::Intersection> xs = s.Intersect(r);
    EXPECT_DOUBLE_EQ(xs.at(0).t_, -1.0);
    EXPECT_DOUBLE_EQ(xs.at(1).t_, 1.0);
}

TEST(SphereTest, TestSphereIsBehindRay) {
    commontypes::Ray r{commontypes::Point{0, 0, 5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere s{};
    const std::vector<geometry::Intersection> xs = s.Intersect(r);
    EXPECT_DOUBLE_EQ(xs.at(0).t_, -6.0);
    EXPECT_DOUBLE_EQ(xs.at(1).t_, -4.0);
}
