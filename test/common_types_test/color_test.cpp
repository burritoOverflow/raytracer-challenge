#include "color.h"
#include <gtest/gtest.h>

TEST(ColorTests, TestColorsAreRGBTuples) {
    commontypes::Color c{-0.5, 0.4, 1.7};
    ASSERT_DOUBLE_EQ(c.Red(), -0.5);
    ASSERT_DOUBLE_EQ(c.Green(), 0.4);
    ASSERT_DOUBLE_EQ(c.Blue(), 1.7);
}

TEST(ColorTests, TestAddingColors) {
    commontypes::Color c1{0.9, 0.6, 0.75};
    commontypes::Color c2{0.7, 0.1, 0.25};
    commontypes::Color expected{1.6, 0.7, 1.0};
    ASSERT_TRUE(c1 + c2 == expected);
    ASSERT_TRUE((c1 += c2) == expected);
}

TEST(ColorTests, TestSubtractingColors) {
    commontypes::Color c1{0.9, 0.6, 0.75};
    commontypes::Color c2{0.7, 0.1, 0.25};
    commontypes::Color expected{0.2, 0.5, 0.5};
    ASSERT_TRUE(c1 - c2 == expected);
    ASSERT_TRUE((c1 - c2) == expected);
}

TEST(ColorTests, TestMultiplyColorsByScalar) {
    commontypes::Color c{0.2, 0.3, 0.4};
    commontypes::Color expected{0.4, 0.6, 0.8};
    ASSERT_TRUE(c * 2 == expected);
}
TEST(ColorTests, TestMultiplyColorByColor) {
    commontypes::Color c1{1, 0.2, 0.4};
    commontypes::Color c2{0.9, 1, 0.1};
    commontypes::Color expected{0.9, 0.2, 0.04};
    ASSERT_TRUE(c1 * c2 == expected);
}
