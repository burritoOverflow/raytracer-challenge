#include "cylinder.h"
#include <gtest/gtest.h>

TEST(CylinderTest, TestRayMissesCylinder) {
    struct Expected {
        const commontypes::Point origin;
        const commontypes::Vector direction;
    };

    const geometry::Cylinder cyl{};
    const std::vector<Expected> expected_values{
        {commontypes::Point{1, 0, 0}, commontypes::Vector{0, 1, 0}},
        {commontypes::Point{0, 0, 0}, commontypes::Vector{0, 1, 0}},
        {commontypes::Point{0, 0, -5}, commontypes::Vector{1, 1, 1}}};

    for (const auto expected : expected_values) {
        const commontypes::Vector direction = commontypes::Vector{expected.direction.Normalize()};
        const commontypes::Ray r{expected.origin, direction};
        const auto xs = cyl.LocalIntersect(r);
        ASSERT_EQ(xs.size(), 0);
    }
}

TEST(CylinderTest, TestRayIntersectsACylinder) {
    struct Expected {
        const commontypes::Point origin;
        const commontypes::Vector direction;
        const double t0;
        const double t1;
    };

    const geometry::Cylinder cyl{};
    const std::vector<Expected> expected_values{
        {commontypes::Point{1, 0, -5}, commontypes::Vector{0, 0, 1}, 5.0, 5.0},
        {commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}, 4.0, 6.0},
        {commontypes::Point{0.5, 0, -5}, commontypes::Vector{0.1, 1, 1}, 6.8079819170273197,
         7.0887234393788612},  // use these longer values here for the purposes of the following
                               // assertion
    };

    for (const auto expected : expected_values) {
        const commontypes::Vector direction = commontypes::Vector{expected.direction.Normalize()};
        const commontypes::Ray r{expected.origin, direction};
        const auto xs = cyl.LocalIntersect(r);

        ASSERT_EQ(xs.size(), 2);
        ASSERT_DOUBLE_EQ(xs.at(0).t_, expected.t0);
        ASSERT_DOUBLE_EQ(xs.at(1).t_, expected.t1);
    }
}

// use points on the surface: +x, -x, +z, -z
TEST(CylinderTest, TestNormalVectorOnCylinder) {
    struct Expected {
        const commontypes::Point point;
        const commontypes::Vector normal;
    };

    const std::vector<Expected> expected_values{
        {commontypes::Point{1, 0, 0}, commontypes::Vector{1, 0, 0}},
        {commontypes::Point{0, 5, -1}, commontypes::Vector{0, 0, -1}},
        {commontypes::Point{0, -2, 1}, commontypes::Vector{0, 0, 1}},
        {commontypes::Point{-1, 1, 0}, commontypes::Vector{-1, 0, 0}},
    };

    const geometry::Cylinder cyl{};

    for (const auto& expected : expected_values) {
        const auto n = cyl.LocalNormalAt(expected.point);
        ASSERT_TRUE(n == expected.normal);
    }
}

TEST(CylinderTest, TestDefaultMinimumAndMaximumOfCylinder) {
    const geometry::Cylinder cyl{};
    const auto inf = std::numeric_limits<double>::infinity();
    ASSERT_DOUBLE_EQ(cyl.Minimum(), -inf);
    ASSERT_DOUBLE_EQ(cyl.Maximum(), inf);
}

TEST(CylinderTest, DefaultClosedValueForCylinder) {
    const geometry::Cylinder cyl{};
    ASSERT_EQ(cyl.IsCapped(), false);
}

TEST(CylinderTest, TestIntersectingaConstrainedCylinder) {
    struct Expected {
        const commontypes::Point origin;
        const commontypes::Vector direction;
        const size_t count;
    };

    geometry::Cylinder cyl{};
    cyl.SetMinimum(1);
    cyl.SetMaximum(2);

    // see descriptions on pg. 183
    // 1. Ray diagonally from inside the Cylinder w/ Ray escaping w/o intersecting the Cylinder
    // 2,3. cast Rays perpendicularly to y-axis from above and below the Cylinder (also misses)
    // 4,5. edge cases where min and mx y-values are outside the bounds of the Cylinder
    // 6. casts Ray perpendicularly through middle of Cylinder producing two intersections

    const std::vector<Expected> expected_values{
        {commontypes::Point{0, 1.5, 0}, commontypes::Vector{0.1, 1, 0}, 0},
        {commontypes::Point{0, 3, -5}, commontypes::Vector{0, 0, 1}, 0},
        {commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}, 0},
        {commontypes::Point{0, 2, -5}, commontypes::Vector{0, 0, 1}, 0},
        {commontypes::Point{0, 1, -5}, commontypes::Vector{0, 0, 1}, 0},
        {commontypes::Point{0, 1.5, -2}, commontypes::Vector{0, 0, 1}, 2},
    };

    for (const auto expected : expected_values) {
        const commontypes::Vector direction = commontypes::Vector{expected.direction.Normalize()};
        const commontypes::Ray r{expected.origin, direction};
        const auto xs = cyl.LocalIntersect(r);
        ASSERT_EQ(xs.size(), expected.count);
    }
}

TEST(CylinderTest, TestIntersectingTheCapsOfClosedCylinder) {
    struct Expected {
        const commontypes::Point origin;
        const commontypes::Vector direction;
        const size_t count;
    };

    const geometry::Cylinder cyl{1, 2, true};

    // see: pg. 185
    // first example casts a Ray from above the Cylinder and points down through the Cylinder's
    // middle along the y-axis. Examples 2 and 4 are above and below the Cylinder (respectively)
    // and cast Ray diagonally through it, intersecting at one end cap and before exiting the far
    // end of the cylinder 3 and 5 - corner cases: intersect an end cap, but exit at the point
    // where the other end cap intersects the side of the Cylinder (in these two, the first
    // intersection is at the first end cap and the second is where the second end cap meets the
    // Cylinder wall.
    const std::vector<Expected> expected_values = {
        {commontypes::Point{0, 3, 0}, commontypes::Vector{0, -1, 0}, 2},
        {commontypes::Point{0, 3, -2}, commontypes::Vector{0, -1, 2}, 2},
        {commontypes::Point{0, 4, -2}, commontypes::Vector{0, -1, 1},
         2},  // corner case - above Cylinder
        {commontypes::Point{0, 0, -2}, commontypes::Vector{0, 1, 2}, 2},
        {commontypes::Point{0, -1, -2}, commontypes::Vector{0, 1, 1},
         2},  // corner case - below Cylinder
    };

    for (const auto& expected : expected_values) {
        const commontypes::Vector direction = commontypes::Vector{expected.direction.Normalize()};
        const commontypes::Ray r{expected.origin, direction};
        const auto xs = cyl.LocalIntersect(r);
        ASSERT_EQ(xs.size(), expected.count);
    }
}

TEST(CylinderTest, TestTheNormalVectorOnCylindersEndCaps) {
    struct Expected {
        const commontypes::Point point;
        const commontypes::Vector normal;
    };

    const std::vector<Expected> expected_values{
        {commontypes::Point{0, 1, 0}, commontypes::Vector{0, -1, 0}},
        {commontypes::Point{0.5, 1, 0}, commontypes::Vector{0, -1, 0}},
        {commontypes::Point{0, 1, 0.5}, commontypes::Vector{0, -1, 0}},
        {commontypes::Point{0, 2, 0}, commontypes::Vector{0, 1, 0}},
        {commontypes::Point{0.5, 2, 0}, commontypes::Vector{0, 1, 0}},
        {commontypes::Point{0, 2, 0.5}, commontypes::Vector{0, 1, 0}},
    };

    const geometry::Cylinder cyl{1, 2, true};

    for (const auto& expected : expected_values) {
        const auto n = cyl.LocalNormalAt(expected.point);
        ASSERT_TRUE(n == expected.normal);
    }
}