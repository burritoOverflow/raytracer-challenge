
#include "ringpattern.h"
#include <gtest/gtest.h>
#include "pattern.h"
#include "point.h"

TEST(RingPatternTest, TestRingShouldExtendInBothXandY) {
    pattern::RingPattern pattern{commontypes::Color::MakeWhite(), commontypes::Color::MakeBlack()};
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 0)) == commontypes::Color::MakeWhite());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(1, 0, 0)) == commontypes::Color::MakeBlack());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0, 0, 1)) == commontypes::Color::MakeBlack());
    ASSERT_TRUE(pattern.PatternAt(commontypes::Point(0.708, 0, 0.708)) ==
                commontypes::Color::MakeBlack());
}