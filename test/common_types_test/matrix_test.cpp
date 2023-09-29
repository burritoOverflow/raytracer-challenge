#include "matrix.h"
#include <gtest/gtest.h>

TEST(MatrixTests, TestConstructAndInspect4x4Matrix) {
    commontypes::Matrix matrix(
        {{1, 2, 3, 4}, {5.5, 6.5, 7.5, 8.5}, {9, 10, 11, 12}, {13.5, 14.5, 15.5, 16.5}});
    ASSERT_DOUBLE_EQ(1, matrix.GetElement(0, 0));
    ASSERT_DOUBLE_EQ(4, matrix.GetElement(0, 3));
    ASSERT_DOUBLE_EQ(5.5, matrix.GetElement(1, 0));
    ASSERT_DOUBLE_EQ(7.5, matrix.GetElement(1, 2));
    ASSERT_DOUBLE_EQ(11, matrix.GetElement(2, 2));
    ASSERT_DOUBLE_EQ(13.5, matrix.GetElement(3, 0));
    ASSERT_DOUBLE_EQ(15.5, matrix.GetElement(3, 2));
}

TEST(MatrixTests, Test2x2MatrixRepresentable) {
    commontypes::Matrix matrix({{-3, 5}, {1, -2}});
    ASSERT_DOUBLE_EQ(-3, matrix.GetElement(0, 0));
    ASSERT_DOUBLE_EQ(5, matrix.GetElement(0, 1));
    ASSERT_DOUBLE_EQ(1, matrix.GetElement(1, 0));
    ASSERT_DOUBLE_EQ(-2, matrix.GetElement(1, 1));
}

TEST(MatrixTests, Test3x3MatrixRepresentable) {
    commontypes::Matrix matrix({{-3, 5, 0}, {1, -2, 7}, {0, 1, 1}});
    ASSERT_DOUBLE_EQ(-3, matrix.GetElement(0, 0));
    ASSERT_DOUBLE_EQ(-2, matrix.GetElement(1, 1));
    ASSERT_DOUBLE_EQ(1, matrix.GetElement(2, 2));
}

TEST(MatrixTests, TestMatrixEqualityWithIdenticalMatrices) {
    commontypes::Matrix a({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}});
    commontypes::Matrix b({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}});
    ASSERT_TRUE(a == b);
}

TEST(MatrixTests, TestMatrixEqualityWithDifferentMatrices) {
    commontypes::Matrix a({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}});
    commontypes::Matrix b({{2, 3, 4, 5}, {6, 7, 8, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}});
    ASSERT_TRUE(a != b);
}
