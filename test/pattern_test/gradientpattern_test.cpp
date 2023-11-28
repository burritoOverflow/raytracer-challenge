#include "gradientpattern.h"
#include <gtest/gtest.h>
#include "color.h"
#include "point.h"

TEST(GradientPatternTest, TestGradientLinearlyInterpolatesBetweenColors) {
    pattern::GradientPattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point{0, 0, 0}) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point{0.25, 0, 0}) ==
                commontypes::Color(0.75, 0.75, 0.75));
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point{0.5, 0, 0}) ==
                commontypes::Color(0.5, 0.5, 0.5));
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point{0.75, 0, 0}) ==
                commontypes::Color(0.25, 0.25, 0.25));
}