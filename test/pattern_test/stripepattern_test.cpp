#include "stripepattern.h"
#include <gtest/gtest.h>
#include "color.h"
#include "point.h"

TEST(StripePatternTest, TestCreatingStripePattern) {
    const pattern::StripePattern pattern{commontypes::Color::MakeWhite(),
                                         commontypes::Color::MakeBlack()};
    ASSERT_TRUE(pattern.ColorA() == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.ColorB() == commontypes::Color::MakeBlack());
}

TEST(StripePatternTest, TestStripePatternConstantInY) {
    const pattern::StripePattern pattern{commontypes::Color::MakeWhite(),
                                         commontypes::Color::MakeBlack()};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 1, 0)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 2, 0)) == commontypes::Color::MakeWhite());
}

TEST(StripePatternTest, TestStripePatternConstantInZ) {
    const pattern::StripePattern pattern{commontypes::Color::MakeWhite(),
                                         commontypes::Color::MakeBlack()};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 1)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 2)) == commontypes::Color::MakeWhite());
}

TEST(StripePatternTest, TestStripePatternAlternatesInX) {
    const pattern::StripePattern pattern{commontypes::Color::MakeWhite(),
                                         commontypes::Color::MakeBlack()};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.5, 0, 0)) ==
                commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.9, 0, 0)) ==
                commontypes::Color::MakeWhite());

    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1, 0, 0)) == commontypes::Color::MakeBlack());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1.1, 0, 0)) ==
                commontypes::Color::MakeBlack());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-0.1, 0, 0)) ==
                commontypes::Color::MakeBlack());

    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-1, 0, 0)) ==
                commontypes::Color::MakeBlack());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(-1.1, 0, 0)) ==
                commontypes::Color::MakeWhite());
}