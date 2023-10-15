#include "sphere.h"
#include <gtest/gtest.h>
#include "ray.h"
#include "scalingmatrix.h"
#include "translationmatrix.h"

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

TEST(SphereTest, TestSpheresDefaultTranslation) {
    geometry::Sphere s{};
    commontypes::IdentityMatrix im{};
    ASSERT_TRUE(s.transform() == im);
}

TEST(SphereTest, TestChangingSpheresTransformation) {
    geometry::Sphere s{};
    commontypes::TranslationMatrix t{2, 3, 4};
    s.SetTransform(t);
    ASSERT_TRUE(s.transform() == t);
}

TEST(SphereTest, TestIntersectingScaledSphereWithRay) {
    commontypes::Ray r{{0, 0, -5}, {0, 0, 1}};
    geometry::Sphere s;
    s.SetTransform(commontypes::ScalingMatrix{2, 2, 2});
    std::vector<geometry::Intersection> xs = s.Intersect(r);
    ASSERT_EQ(xs.size(), 2);
    ASSERT_DOUBLE_EQ(xs.at(0).t_, 3);
    ASSERT_DOUBLE_EQ(xs.at(1).t_, 7);
}

TEST(SphereTest, TestIntersectingTranslatedSphereWithRay) {
    commontypes::Ray r{{0, 0, -5}, {0, 0, 1}};
    geometry::Sphere s;
    s.SetTransform(commontypes::TranslationMatrix{5, 0, 0});
    std::vector<geometry::Intersection> xs = s.Intersect(r);
    ASSERT_EQ(xs.size(), 0);
}