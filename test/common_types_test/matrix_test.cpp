#include <gtest/gtest.h>
#include "identitymatrix.h"

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

TEST(MatrixTests, TestMatrixMultiplicationForTwoMatrices) {
    commontypes::Matrix a({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}});
    commontypes::Matrix b({{-2, 1, 2, 3}, {3, 2, 1, -1}, {4, 3, 6, 5}, {1, 2, 7, 8}});
    commontypes::Matrix expected(
        {{20, 22, 50, 48}, {44, 54, 114, 108}, {40, 58, 110, 102}, {16, 26, 46, 42}});
    ASSERT_TRUE(a * b == expected);
}

TEST(MatrixTests, TestMatrixMultiplyByTuple) {
    commontypes::Matrix a({{1, 2, 3, 4}, {2, 4, 4, 2}, {8, 6, 4, 1}, {0, 0, 0, 1}});
    commontypes::Tuple b{1, 2, 3, 1};
    commontypes::Tuple expected{18, 24, 33, 1};
    ASSERT_TRUE(a * b == expected);
}

TEST(MatrixTests, TestMatrixMultiplyMatrixByIdentityMatrix) {
    commontypes::Matrix a({{0, 1, 2, 4}, {1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}});
    commontypes::IdentityMatrix identity_matrix{};
    ASSERT_TRUE(a * identity_matrix == a);
    ASSERT_TRUE(identity_matrix * a == a);
}

TEST(MatrixTests, TestMatrixMultiplyTupleByIdentityMatrix) {
    commontypes::Tuple a{1, 2, 3, 4};
    commontypes::IdentityMatrix identity_matrix{};
    ASSERT_TRUE(a * identity_matrix == a);
    ASSERT_TRUE(identity_matrix * a == a);
}

TEST(MatrixTests, TestMatrixTranspose) {
    commontypes::Matrix a({{0, 9, 3, 0}, {9, 8, 0, 8}, {1, 8, 5, 3}, {0, 0, 5, 8}});
    commontypes::Matrix a_transpose({{0, 9, 1, 0}, {9, 8, 8, 0}, {3, 0, 5, 5}, {0, 8, 3, 8}});
    ASSERT_TRUE(a.Transpose() == a_transpose);
}

TEST(MatrixTests, TestMatrix2x2Determinant) {
    commontypes::Matrix a({{1, 5}, {-3, 2}});
    const double expected = 17.0;
    ASSERT_DOUBLE_EQ(a.Determinant(), expected);
}

TEST(MatrixTests, TestSubmatrixOf3x3MatrixIs2x2Matrix) {
    commontypes::Matrix a({{1, 5, 0}, {-3, 2, 7}, {0, 6, -3}});
    commontypes::Matrix expected_submatrix({{-3, 2}, {0, 6}});
    commontypes::Matrix result = a.Submatrix(0, 2);
    ASSERT_TRUE(result == expected_submatrix);
}

TEST(MatrixTests, TestSubmatrixOf4x4MatrixIs3x3Matrix) {
    commontypes::Matrix a({{-6, 1, 1, 6}, {-8, 5, 8, 6}, {-1, 0, 8, 2}, {-7, 1, -1, 1}});
    commontypes::Matrix expected_submatrix({{-6, 1, 6}, {-8, 8, 6}, {-7, -1, 1}});
    commontypes::Matrix result = a.Submatrix(2, 1);
    ASSERT_TRUE(result == expected_submatrix);
}

TEST(MatrixTests, TestCalculateMinorOf3x3Matrix) {
    commontypes::Matrix a({{3, 5, 0}, {2, -1, 7}, {6, -1, 5}});
    commontypes::Matrix b = a.Submatrix(1, 0);
    const double expected_determinant{25.0};
    ASSERT_TRUE(b.Determinant() == expected_determinant);
    ASSERT_TRUE(a.Minor(1, 0) == expected_determinant);
    ASSERT_TRUE(b.Determinant() == a.Minor(1, 0));
}

TEST(MatrixTests, TestCalculateCofactorOf3x3Matrix) {
    commontypes::Matrix a({{3, 5, 0}, {2, -1, -7}, {6, -1, 5}});
    const double expected{-12};
    EXPECT_DOUBLE_EQ(expected, a.Minor(0, 0));
    EXPECT_DOUBLE_EQ(a.Minor(1, 0), 25.0);
    EXPECT_DOUBLE_EQ(a.Cofactor(1, 0), -25.0);
}

TEST(MatrixTests, TestDeterminantOf3x3Matrix) {
    commontypes::Matrix a({{1, 2, 6}, {-5, 8, -4}, {2, 6, 4}});
    ASSERT_DOUBLE_EQ(a.Cofactor(0, 0), 56.0);
    ASSERT_DOUBLE_EQ(a.Cofactor(0, 1), 12.0);
    ASSERT_DOUBLE_EQ(a.Cofactor(0, 2), -46.0);
    ASSERT_DOUBLE_EQ(a.Determinant(), -196);
}

TEST(MatrixTests, TestDeterminantOf4x4Matrix) {
    commontypes::Matrix a({{-2, -8, 3, 5}, {-3, 1, 7, 3}, {1, 2, -9, 6}, {-6, 7, 7, -9}});
    ASSERT_DOUBLE_EQ(a.Cofactor(0, 0), 690.0);
    ASSERT_DOUBLE_EQ(a.Cofactor(0, 1), 447.0);
    ASSERT_DOUBLE_EQ(a.Cofactor(0, 2), 210.0);
    ASSERT_DOUBLE_EQ(a.Cofactor(0, 3), 51.0);
    ASSERT_DOUBLE_EQ(a.Determinant(), -4071.0);
}

TEST(MatrixTests, TestInvertibleMatrixForInvertibility) {
    commontypes::Matrix a({{6, 4, 4, 4}, {5, 5, 7, 6}, {4, -9, 3, -7}, {9, 1, 7, -6}});
    ASSERT_DOUBLE_EQ(a.Determinant(), -2120.0);
    ASSERT_TRUE(a.IsInvertible());
}

TEST(MatrixTests, TestNonInvertibleMatrixForInvertibility) {
    commontypes::Matrix a({{-4, 2, -2, -3}, {9, 6, 2, 6}, {0, -5, 1, -5}, {0, 0, 0, 0}});
    ASSERT_DOUBLE_EQ(a.Determinant(), 0.0);
    ASSERT_FALSE(a.IsInvertible());
}
