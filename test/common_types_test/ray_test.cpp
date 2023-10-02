#include "ray.h"
#include <gtest/gtest.h>

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