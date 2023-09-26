#include "tuple.h"
#include <gtest/gtest.h>

TEST(TupleTests, TestTupleIsPoint) {
    commontypes::Tuple a{4.3, -4.2, 3.1, 1.0};

    ASSERT_DOUBLE_EQ(a.x(), 4.3);
    ASSERT_DOUBLE_EQ(a.y(), -4.2);
    ASSERT_DOUBLE_EQ(a.z(), 3.1);
    ASSERT_DOUBLE_EQ(a.w(), 1.0);

    EXPECT_TRUE(a.IsPoint());
    EXPECT_FALSE(a.IsVector());
}

TEST(TupleTests, TestTupleIsVector) {
    commontypes::Tuple a{4.3, -4.2, 3.1, 0.0};

    ASSERT_DOUBLE_EQ(a.x(), 4.3);
    ASSERT_DOUBLE_EQ(a.y(), -4.2);
    ASSERT_DOUBLE_EQ(a.z(), 3.1);
    ASSERT_DOUBLE_EQ(a.w(), 0.0);

    EXPECT_FALSE(a.IsPoint());
    EXPECT_TRUE(a.IsVector());
}

TEST(TupleTests, TestAddingTwoTuples) {
    commontypes::Tuple a1{3, -2, 5, 1};
    commontypes::Tuple a2{-2, 3, 1, 0};
    commontypes::Tuple expected{1, 1, 6, 1};
    EXPECT_TRUE(a1 + a2 == expected);

    // verify that operator+= functions in the same fashion
    EXPECT_TRUE((a1 += a2) == expected);
}

TEST(TupleTests, TestNegatingTuple) {
    commontypes::Tuple a{1, -2, 3, -4};
    commontypes::Tuple expected{-1, 2, -3, 4};
    EXPECT_TRUE(-a == expected);
}

TEST(TupleTests, TestMultiplyTupleByScalar) {
    commontypes::Tuple a{1, -2, 3, -4};
    commontypes::Tuple expected{3.5, -7, 10.5, -14};
    EXPECT_TRUE(a * 3.5 == expected);
}

TEST(TupleTests, TestMultiplyTupleByFraction) {
    commontypes::Tuple a{1, -2, 3, -4};
    commontypes::Tuple expected{0.5, -1, 1.5, -2};
    EXPECT_TRUE(a * 0.5 == expected);
}

TEST(TupleTests, TestDivideTupleByScalar) {
    commontypes::Tuple a{1, -2, 3, -4};
    commontypes::Tuple expected{0.5, -1, 1.5, -2};
    EXPECT_TRUE(a / 2 == expected);
}
