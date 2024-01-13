#include "cone.h"
#include <gtest/gtest.h>

TEST(ConeTest, TestCreatingNewCone) {
    geometry::Cone c{};
    ASSERT_EQ(c.IsCapped(), false);
    ASSERT_TRUE(c.GetTransform() == commontypes::IdentityMatrix());
}

TEST(ConeTest, TestIntersectingConeWithRay) {
    struct Expected {
        const commontypes::Point origin;
        const commontypes::Vector direction;
        const double t0;
        const double t1;
    };

    geometry::Cone shape{};

    // use values with greater precision than the book to avoid floating point comparison issues
    const std::vector<Expected> expected_values{
        {commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}, 5.0, 5.0},
        {commontypes::Point{0, 0, -5}, commontypes::Vector{1, 1, 1}, 8.6602540378443855,
         8.6602540378443855},
        {commontypes::Point{1, 1, -5}, commontypes::Vector{-0.5, -1, 1}, 4.5500556793563494,
         49.449944320643645},
    };

    for (const auto expected : expected_values) {
        const commontypes::Vector direction = commontypes::Vector{expected.direction.Normalize()};
        const commontypes::Ray r{expected.origin, direction};
        const auto xs = shape.LocalIntersect(r);

        ASSERT_EQ(xs.size(), 2);
        ASSERT_DOUBLE_EQ(xs.at(0).t_, expected.t0);
        ASSERT_DOUBLE_EQ(xs.at(1).t_, expected.t1);
    }
}

TEST(ConeTest, TestIntersectingConeWithRayParallelToOneOfItsHalves) {
    geometry::Cone shape{};
    const commontypes::Vector direction =
        commontypes::Vector{commontypes::Vector{0, 1, 1}.Normalize()};

    const commontypes::Ray ray{commontypes::Point{0, 0, -1}, direction};
    const auto xs = shape.LocalIntersect(ray);

    ASSERT_EQ(xs.size(), 1);
    ASSERT_DOUBLE_EQ(xs.at(0).t_, 0.35355339059327379);
}

TEST(ConeTest, TestIntersectingConesEndCaps) {
    struct Expected {
        const commontypes::Point origin;
        const commontypes::Vector direction;
        const size_t count;
    };

    geometry::Cone cone{-0.5, 0.5, true};

    const std::vector<Expected> expected_values = {
        {commontypes::Point{0, 0, -5}, commontypes::Vector{0, 1, 0}, 0},
        {commontypes::Point{0, 0, -0.25}, commontypes::Vector{0, 1, 1}, 2},
        {commontypes::Point{0, 0, -0.25}, commontypes::Vector{0, 1, 0}, 4},
    };

    for (const auto& expected : expected_values) {
        const commontypes::Vector direction = commontypes::Vector{expected.direction.Normalize()};
        commontypes::Ray r{expected.origin, direction};
        const auto xs = cone.LocalIntersect(r);

        ASSERT_EQ(xs.size(), expected.count);
    }
}

TEST(ConeTest, TestNormalVectorOnCone) {
    struct Expected {
        const commontypes::Point point;
        const commontypes::Vector normal;
    };

    const std::vector<Expected> expected_values{
        {commontypes::Point{0, 0, 0}, commontypes::Vector{0, 0, 0}},
        {commontypes::Point{1, 1, 1}, commontypes::Vector{1, -sqrt(2), 1}},
        {commontypes::Point{-1, -1, 0}, commontypes::Vector{-1, 1, 0}},
    };

    geometry::Cone cyl{};

    for (const auto& expected : expected_values) {
        const auto n = cyl.LocalNormalAt(expected.point);

        ASSERT_TRUE(n == expected.normal);
    }
}