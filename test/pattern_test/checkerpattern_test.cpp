#include "checkerpattern.h"
#include <gtest/gtest.h>
#include "checkerpattern.h"
#include "pattern.h"

TEST(CheckerPatternTest, TestCheckersShouldRepeatInX) {
    pattern::CheckerPattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.99, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1.01, 0, 0)) == pattern::BLACK);
}

TEST(CheckerPatternTest, TestCheckersShouldRepeatInY) {
    pattern::CheckerPattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0.99, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 1.01, 0)) == pattern::BLACK);
}

TEST(CheckerPatternTest, TestCheckersShouldRepeatInZ) {
    pattern::CheckerPattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0.99)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 1.01)) == pattern::BLACK);
}