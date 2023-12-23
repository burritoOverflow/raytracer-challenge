#include "cube.h"
#include <gtest/gtest.h>

TEST(CubeTest, TestRayIntersectsACube) {
    struct ExpectedResult {
        commontypes::Point origin;
        commontypes::Vector direction;
        double t0;  // first intersection
        double t1;  // second intersection
    };

    // see pg. 168-169
    std::vector<ExpectedResult> expected_results = {
        /*+x*/ {commontypes::Point{5, 0.5, 0}, commontypes::Vector{-1, 0, 0}, 4, 6},
        /*-x*/ {commontypes::Point{-5, 0.5, 0}, commontypes::Vector{1, 0, 0}, 4, 6},
        /*+y*/ {commontypes::Point{0.5, 5, 0}, commontypes::Vector{0, -1, 0}, 4, 6},
        /*-y*/ {commontypes::Point{0.5, -5, 0}, commontypes::Vector{0, 1, 0}, 4, 6},
        /*+z*/ {commontypes::Point{0.5, 0, 5}, commontypes::Vector{0, 0, -1}, 4, 6},
        /*-z*/ {commontypes::Point{0.5, 0, -5}, commontypes::Vector{0, 0, 1}, 4, 6},
        /*inside*/ {commontypes::Point{0, 0.5, 0}, commontypes::Vector{0, 0, 1}, -1, 1}};

    for (const auto& expected_result : expected_results) {
        geometry::Cube c;
        const commontypes::Ray ray{expected_result.origin, expected_result.direction};
        const auto xs = c.LocalIntersect(ray);
        EXPECT_EQ(xs.size(), 2);
        EXPECT_DOUBLE_EQ(xs.at(0).t_, expected_result.t0);
        EXPECT_DOUBLE_EQ(xs.at(1).t_, expected_result.t1);
    }
}

TEST(CubeTest, TestRayMissesACube) {
    struct ExpectedResult {
        commontypes::Point origin;
        commontypes::Vector direction;
    };

    // see pg. 168-169
    std::vector<ExpectedResult> expected_results = {
        ExpectedResult{commontypes::Point{-2, 0, 0}, commontypes::Vector{0.2673, 0.5345, 0.8018}},
        ExpectedResult{commontypes::Point{0, -2, 0}, commontypes::Vector{0.8018, 0.2673, 0.5345}},
        ExpectedResult{commontypes::Point{0, 0, -2}, commontypes::Vector{0.5345, 0.8018, 0.2673}},
        ExpectedResult{commontypes::Point{2, 0, 2}, commontypes::Vector{0, 0, -1}},
        ExpectedResult{commontypes::Point{0, 2, 2}, commontypes::Vector{0, -1, 0}},
        ExpectedResult{commontypes::Point{2, 2, 0}, commontypes::Vector{-1, 0, 0}}};

    for (const auto& expected_result : expected_results) {
        geometry::Cube c;
        const commontypes::Ray ray{expected_result.origin, expected_result.direction};
        const auto xs = c.LocalIntersect(ray);
        EXPECT_EQ(xs.size(), 0);  // expected to miss the cube, thus no intersections
    }
}

TEST(CubeTest, TestNormalOnSurfaceOfACube) {
    // see table on pg. 147
    struct ExpectedResult {
        commontypes::Point point;
        commontypes::Vector normal;  // expected result
    };

    std::vector<ExpectedResult> expected_results = {
        {commontypes::Point{1, 0.5, -0.8}, commontypes::Vector{1, 0, 0}},
        {commontypes::Point{-1, -0.2, 0.9}, commontypes::Vector{-1, 0, 0}},
        {commontypes::Point{-0.4, 1, -0.1}, commontypes::Vector{0, 1, 0}},
        {commontypes::Point{0.3, -1, -0.7}, commontypes::Vector{0, -1, 0}},
        {commontypes::Point{-0.6, 0.3, 1}, commontypes::Vector{0, 0, 1}},
        {commontypes::Point{0.4, 0.4, -1}, commontypes::Vector{0, 0, -1}},
        {commontypes::Point{1, 1, 1}, commontypes::Vector{1, 0, 0}},
        {commontypes::Point{-1, -1, -1}, commontypes::Vector{-1, 0, 0}},
    };

    for (const auto& expected_result : expected_results) {
        geometry::Cube c;
        const auto normal = c.LocalNormalAt(expected_result.point);
        EXPECT_TRUE(expected_result.normal == normal);
    }
}