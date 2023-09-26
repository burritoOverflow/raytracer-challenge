#include "vector.h"
#include <gtest/gtest.h>
#include "point.h"

TEST(VectorTests, TestSubtractVectorFromPoint) {
    commontypes::Point p{3, 2, 1};
    commontypes::Vector v{5, 6, 7};
    commontypes::Point expected{-2, -4, -6};
    EXPECT_TRUE(p - v == expected);

    // ensure subtraction of Point and Vector is a Point
    commontypes::Tuple result = p - v;
    EXPECT_TRUE(result.IsPoint());
    EXPECT_TRUE((p -= v) == expected);
}

TEST(VectorTests, TestSubtractVectorFromVector) {
    commontypes::Vector v1{3, 2, 1};
    commontypes::Vector v2{5, 6, 7};
    commontypes::Vector expected{-2, -4, -6};
    EXPECT_TRUE(v1 - v2 == expected);

    // Vector - Vector subtraction results in a Vector
    commontypes::Tuple result = v1 - v2;
    EXPECT_TRUE(result.IsVector());
    EXPECT_TRUE((v1 -= v2) == expected);
}

TEST(VectorTests, TestSubtractVectorFromZeroVector) {
    commontypes::Vector zero_vector{0, 0, 0};
    commontypes::Vector v{1, -2, 3};
    commontypes::Vector expected{-1, 2, -3};

    EXPECT_TRUE(zero_vector - v == expected);
    EXPECT_TRUE((zero_vector -= v) == expected);
}

TEST(VectorTests, TestComputeMagnitudeOfVector) {
    commontypes::Vector v1{1, 0, 0};
    EXPECT_DOUBLE_EQ(v1.Magnitude(), 1);

    commontypes::Vector v2{0, 1, 0};
    EXPECT_DOUBLE_EQ(v2.Magnitude(), 1);

    commontypes::Vector v3{0, 0, 1};
    EXPECT_DOUBLE_EQ(v3.Magnitude(), 1);

    commontypes::Vector v4{1, 2, 3};
    EXPECT_DOUBLE_EQ(v4.Magnitude(), sqrt(14));

    commontypes::Vector v5{-1, -2, -3};
    EXPECT_DOUBLE_EQ(v5.Magnitude(), sqrt(14));
}

TEST(VectorTests, TestNormalizingVector) {
    commontypes::Vector v1{4, 0, 0};
    commontypes::Vector expected1{1, 0, 0};
    EXPECT_TRUE(v1.Normalize() == expected1);

    commontypes::Vector v2{1, 2, 3};
    commontypes::Vector expected2{0.26726, 0.53452, 0.80178};
    commontypes::Tuple normalized = v2.Normalize();
    EXPECT_TRUE(normalized == expected2);
}

TEST(VectorTests, TestDotProduct) {
    commontypes::Vector a{1, 2, 3};
    commontypes::Vector b{2, 3, 4};
    const double expected = 20.0;
    EXPECT_EQ(a.Dot(b), expected);
}

TEST(VectorTests, TestCrossProduct) {
    commontypes::Vector a{1, 2, 3};
    commontypes::Vector b{2, 3, 4};
    commontypes::Vector expected1{-1, 2, -1};
    commontypes::Vector expected2{1, -2, 1};
    
    EXPECT_TRUE(a.Cross(b) == expected1);
    EXPECT_TRUE(b.Cross(a) == expected2);
}