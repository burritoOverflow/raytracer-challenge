
#include "ringpattern.h"
#include <gtest/gtest.h>
#include "pattern.h"
#include "point.h"

TEST(RingPatternTest, TestRingShouldExtendInBothXandY) {
    pattern::RingPattern pattern{pattern::WHITE, pattern::BLACK};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == pattern::WHITE);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1, 0, 0)) == pattern::BLACK);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 1)) == pattern::BLACK);
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.708, 0, 0.708)) == pattern::BLACK);
}