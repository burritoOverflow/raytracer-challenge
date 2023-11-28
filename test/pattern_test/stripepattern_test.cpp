#include "stripepattern.h"
#include <gtest/gtest.h>
#include "color.h"
#include "point.h"

TEST(StripePatternTest, TestCreatingStripePattern) {
    const pattern::StripePattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.ColorA() == pattern::WHITE);
    ASSERT_TRUE(pattern.ColorB() == pattern::BLACK);
}

TEST(StripePatternTest, TestStripePatternConstantInY) {
    pattern::StripePattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 1, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 2, 0)) == pattern::WHITE);
}

TEST(StripePatternTest, TestStripePatternConstantInZ) {
    pattern::StripePattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 1)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 2)) == pattern::WHITE);
}

TEST(StripePatternTest, TestStripePatternAlternatesInX) {
    pattern::StripePattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.5, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.9, 0, 0)) == pattern::WHITE);

    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1, 0, 0)) == pattern::BLACK);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1.1, 0, 0)) == pattern::BLACK);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-0.1, 0, 0)) == pattern::BLACK);

    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-1, 0, 0)) == pattern::BLACK);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-1.1, 0, 0)) == pattern::WHITE);
}