#include "checkerpattern.h"
#include <gtest/gtest.h>
#include "checkerpattern.h"
#include "pattern.h"

TEST(CheckerPatternTest, TestCheckersShouldRepeatInX) {
    const pattern::CheckerPattern pattern{commontypes::Color::MakeWhite(),
                                          commontypes::Color::MakeBlack()};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.99, 0, 0)) ==
                commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1.01, 0, 0)) ==
                commontypes::Color::MakeBlack());
}

TEST(CheckerPatternTest, TestCheckersShouldRepeatInY) {
    const pattern::CheckerPattern pattern{commontypes::Color::MakeWhite(),
                                          commontypes::Color::MakeBlack()};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0.99, 0)) ==
                commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 1.01, 0)) ==
                commontypes::Color::MakeBlack());
}

TEST(CheckerPatternTest, TestCheckersShouldRepeatInZ) {
    const pattern::CheckerPattern pattern{commontypes::Color::MakeWhite(),
                                          commontypes::Color::MakeBlack()};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0.99)) ==
                commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 1.01)) ==
                commontypes::Color::MakeBlack());
}