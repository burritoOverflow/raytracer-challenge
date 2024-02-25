#include "point.h"
#include <gtest/gtest.h>
#include "vector.h"

// Scenario : Subtracting two points
TEST(PointTests, TestSubtractTwoPoints) {
    commontypes::Point p1{3, 2, 1};
    commontypes::Point p2{5, 6, 7};
    commontypes::Vector expected{-2, -4, -6};
    EXPECT_TRUE(p1 - p2 == expected);

    // ensure subtraction of two Points is a Vector
    commontypes::Tuple result = p1 - p2;
    EXPECT_TRUE(result.IsVector());

    // verify -= operator behavior
    EXPECT_TRUE((p1 -= p2) == expected);
}
