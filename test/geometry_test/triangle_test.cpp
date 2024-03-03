#include "triangle.h"
#include <gtest/gtest.h>
#include "point.h"

TEST(TriangleTest, TestConstructingTriangle) {
    const commontypes::Point p1{0, 1, 0};
    const commontypes::Point p2{-1, 0, 0};
    const commontypes::Point p3{1, 0, 0};
    const geometry::Triangle t{p1, p2, p3};

    ASSERT_TRUE(t.P1() == p1);
    ASSERT_TRUE(t.P2() == p2);
    ASSERT_TRUE(t.P3() == p3);

    // verify the edge vectors and the normal are precomputed for the Triangle
    ASSERT_TRUE(t.E1() == commontypes::Vector(-1, -1, 0));
    ASSERT_TRUE(t.E2() == commontypes::Vector(1, -1, 0));

    ASSERT_TRUE(t.Normal() == commontypes::Vector(0, 0, -1));
}

TEST(TriangleTest, TestFindingTheNormalOnTriangle) {
    const geometry::Triangle t{commontypes::Point{0, 1, 0}, commontypes::Point{-1, 0, 0},
                               commontypes::Point{1, 0, 0}};

    const commontypes::Vector n1 = t.LocalNormalAt(commontypes::Point{0, 0.5, 0});
    const commontypes::Vector n2 = t.LocalNormalAt(commontypes::Point{-0.5, 0.75, 0});
    const commontypes::Vector n3 = t.LocalNormalAt(commontypes::Point{0.5, 0.25, 0});

    // normal should be the same for all Points
    ASSERT_TRUE(n1 == t.Normal());
    ASSERT_TRUE(n2 == t.Normal());
    ASSERT_TRUE(n3 == t.Normal());
}

// position Ray s.t it is cast parallel to the Triangle; assert that the Ray misses the Triangle
TEST(TriangleTest, TestIntersectingRayParallelToTheTriangle) {
    const geometry::Triangle t{commontypes::Point{0, 1, 0}, commontypes::Point{-1, 0, 0},
                               commontypes::Point{1, 0, 0}};

    const commontypes::Ray r{commontypes::Point{0, -1, -2}, commontypes::Vector{0, 1, 0}};

    const auto xs = t.LocalIntersect(r);
    ASSERT_EQ(xs.size(), 0);
}

// test that the Ray misses when it passes beyond the P3-P1 edge
TEST(TriangleTest, TestRayMissesTheP1P3Edge) {
    const geometry::Triangle t{commontypes::Point{0, 1, 0}, commontypes::Point{-1, 0, 0},
                               commontypes::Point{1, 0, 0}};

    const commontypes::Ray r{commontypes::Point{1, 1, -2}, commontypes::Vector{0, 0, 1}};

    const auto xs = t.LocalIntersect(r);
    ASSERT_EQ(xs.size(), 0);
}

TEST(TriangleTest, TestRayMissesTheP1P2Edge) {
    const geometry::Triangle t{commontypes::Point{0, 1, 0}, commontypes::Point{-1, 0, 0},
                               commontypes::Point{1, 0, 0}};

    const commontypes::Ray r{commontypes::Point{-1, 1, -2}, commontypes::Vector{0, 0, 1}};

    const auto xs = t.LocalIntersect(r);
    ASSERT_EQ(xs.size(), 0);
}

TEST(TriangleTest, TestRayMissesTheP2P3Edge) {
    const geometry::Triangle t{commontypes::Point{0, 1, 0}, commontypes::Point{-1, 0, 0},
                               commontypes::Point{1, 0, 0}};

    const commontypes::Ray r{commontypes::Point{0, -1, -2}, commontypes::Vector{0, 0, 1}};

    const auto xs = t.LocalIntersect(r);
    ASSERT_EQ(xs.size(), 0);
}

TEST(TriangleTest, TestRayStrikesTriangle) {
    const geometry::Triangle t{commontypes::Point{0, 1, 0}, commontypes::Point{-1, 0, 0},
                               commontypes::Point{1, 0, 0}};

    const commontypes::Ray r{commontypes::Point{0, 0.5, -2}, commontypes::Vector{0, 0, 1}};

    const auto xs = t.LocalIntersect(r);
    ASSERT_EQ(xs.size(), 1);
    ASSERT_EQ(xs.at(0).t_, 2);
}