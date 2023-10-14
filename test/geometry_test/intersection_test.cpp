#include "intersection.h"
#include <gtest/gtest.h>
#include "sphere.h"

TEST(IntersectionTest, TestIntersectionEncapsulatesTandObject) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Sphere> s_ptr = std::make_shared<geometry::Sphere>(s);
    double t{3.5};
    geometry::Intersection i{t, s_ptr};
    ASSERT_EQ(i.t_, t);
    ASSERT_EQ(i.object_, s_ptr);
}

TEST(IntersectionTest, TestAggregatingIntersections) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Sphere> s_ptr = std::make_shared<geometry::Sphere>(s);
    geometry::Intersection i1{1, s_ptr};
    geometry::Intersection i2{2, s_ptr};
    std::vector<geometry::Intersection> xs = {i1, i2};
    ASSERT_EQ(xs.size(), 2);
    ASSERT_DOUBLE_EQ(xs.at(0).t_, 1);
    ASSERT_DOUBLE_EQ(xs.at(1).t_, 2);
}

TEST(IntersectionTest, TestIntersectSetsTheObjectOnTheIntersection) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere s{};
    std::shared_ptr<geometry::Sphere> s_ptr = std::make_shared<geometry::Sphere>(s);

    const std::vector<geometry::Intersection> xs = s.Intersect(r);
    ASSERT_EQ(xs.size(), 2);
    ASSERT_TRUE(*xs.at(0).object_ == *s_ptr);
    ASSERT_TRUE(*xs.at(1).object_ == *s_ptr);
}

TEST(IntersectionTest, TestHitWhenAllIntersectionsHavePositiveTValues) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Sphere> s_ptr = std::make_shared<geometry::Sphere>(s);

    geometry::Intersection i1{1, s_ptr};
    geometry::Intersection i2{2, s_ptr};
    std::vector<geometry::Intersection> xs = {i2, i1};

    std::optional<geometry::Intersection> oi = geometry::Intersection::Hit(xs);
    ASSERT_TRUE(oi.has_value());
    geometry::Intersection i = oi.value();
    ASSERT_TRUE(i == i1);
}

TEST(IntersectionTest, TestHitWhenSomeIntersectionsHaveNegativeTValues) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Sphere> s_ptr = std::make_shared<geometry::Sphere>(s);

    geometry::Intersection i1{-1, s_ptr};
    geometry::Intersection i2{1, s_ptr};
    std::vector<geometry::Intersection> xs = {i2, i1};

    std::optional<geometry::Intersection> oi = geometry::Intersection::Hit(xs);
    ASSERT_TRUE(oi.has_value());
    geometry::Intersection i = oi.value();
    ASSERT_TRUE(i == i2);
}

TEST(IntersectionTest, TestHitWhenAllIntersectionsHaveNegativeTValues) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Sphere> s_ptr = std::make_shared<geometry::Sphere>(s);

    geometry::Intersection i1{-2, s_ptr};
    geometry::Intersection i2{-1, s_ptr};
    std::vector<geometry::Intersection> xs = {i2, i1};

    // no hits should be present when t values are all negative
    std::optional<geometry::Intersection> oi = geometry::Intersection::Hit(xs);
    ASSERT_FALSE(oi.has_value());
}

TEST(IntersectionTest, TestHitIsAlwaysTheLowestNonnegativeIntersection) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Sphere> s_ptr = std::make_shared<geometry::Sphere>(s);

    geometry::Intersection i1{5, s_ptr};
    geometry::Intersection i2{7, s_ptr};
    geometry::Intersection i3{-3, s_ptr};
    geometry::Intersection i4{2, s_ptr};
    std::vector<geometry::Intersection> xs{i1, i2, i3, i4};

    std::optional<geometry::Intersection> oi = geometry::Intersection::Hit(xs);
    ASSERT_TRUE(oi.has_value());
    ASSERT_TRUE(oi.value() == i4);
}
