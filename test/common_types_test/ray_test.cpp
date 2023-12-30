#include "ray.h"
#include <gtest/gtest.h>
#include "scalingmatrix.h"
#include "translationmatrix.h"

TEST(RayTests, TestCreatingAndQueryingRay) {
    commontypes::Point origin{1, 2, 3};
    commontypes::Vector direction{4, 5, 6};
    commontypes::Ray r{origin, direction};
    ASSERT_TRUE(r.direction() == direction);
    ASSERT_TRUE(r.origin() == origin);
}

TEST(RayTests, TestComputingPointFromDistance) {
    commontypes::Ray r{commontypes::Point{2, 3, 4}, commontypes::Vector{1, 0, 0}};
    ASSERT_TRUE(r.Position(0) == commontypes::Point(2, 3, 4));
    ASSERT_TRUE(r.Position(1) == commontypes::Point(3, 3, 4));
    ASSERT_TRUE(r.Position(-1) == commontypes::Point(1, 3, 4));
    ASSERT_TRUE(r.Position(2.5) == commontypes::Point(4.5, 3, 4));
}

TEST(RayTests, TestTranslatingRay) {
    commontypes::Ray r{commontypes::Point{1, 2, 3}, {0, 1, 0}};
    commontypes::TranslationMatrix m{3, 4, 5};
    commontypes::Ray r2 = r.Transform(m);
    ASSERT_TRUE(r2.origin() == commontypes::Point(4, 6, 8));
    ASSERT_TRUE(r2.direction() == commontypes::Vector(0, 1, 0));
}

TEST(RayTests, TestScalingRay) {
    commontypes::Ray r{commontypes::Point{1, 2, 3}, {0, 1, 0}};
    commontypes::ScalingMatrix s{2, 3, 4};
    commontypes::Ray r2 = r.Transform(s);
    ASSERT_TRUE(r2.origin() == commontypes::Point(2, 6, 12));
    ASSERT_TRUE(r2.direction() == commontypes::Vector(0, 3, 0));
}