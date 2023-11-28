#include "stripepattern.h"
#include <gtest/gtest.h>
#include "color.h"
#include "point.h"

const commontypes::Color BLACK = {0, 0, 0};
const commontypes::Color WHITE = {1, 1, 1};

TEST(StripePatternTest, TestCreatingStripePattern) {
    const pattern::StripePattern pattern{WHITE, BLACK};
    ASSERT_TRUE(pattern.ColorA() == WHITE);
    ASSERT_TRUE(pattern.ColorB() == BLACK);
}

TEST(StripePatternTest, TestStripePatternConstantInY) {
    pattern::StripePattern pattern{WHITE, BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 1, 0)) == WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 2, 0)) == WHITE);
}

TEST(StripePatternTest, TestStripePatternConstantInZ) {
    pattern::StripePattern pattern{WHITE, BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 1)) == WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 2)) == WHITE);
}

TEST(StripePatternTest, TestStripePatternAlternatesInX) {
    pattern::StripePattern pattern{WHITE, BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.5, 0, 0)) == WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.9, 0, 0)) == WHITE);

    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1, 0, 0)) == BLACK);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1.1, 0, 0)) == BLACK);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-0.1, 0, 0)) == BLACK);

    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-1, 0, 0)) == BLACK);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-1.1, 0, 0)) == WHITE);
}