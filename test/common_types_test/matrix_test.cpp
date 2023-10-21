#include <gtest/gtest.h>
#include "identitymatrix.h"
#include "point.h"
#include "rotationmatrix.h"
#include "scalingmatrix.h"
#include "shearingmatrix.h"
#include "translationmatrix.h"
#include "vector.h"

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

TEST(MatrixTests, TestCalculatingInverseOfMatrix) {
    commontypes::Matrix a({{-5, 2, 6, -8}, {1, -5, 1, 8}, {7, 7, -6, -7}, {1, -3, 7, 4}});
    commontypes::Matrix b = a.Inverse();
    ASSERT_DOUBLE_EQ(a.Determinant(), 532.0);
    ASSERT_DOUBLE_EQ(a.Cofactor(2, 3), -160.0);
    ASSERT_DOUBLE_EQ(b.GetElement(3, 2), -160.0 / 532.0);
    ASSERT_DOUBLE_EQ(a.Cofactor(3, 2), 105.0);
    ASSERT_DOUBLE_EQ(b.GetElement(2, 3), 105.0 / 532.0);

    commontypes::Matrix expected({
        {0.21805, 0.45113, 0.24060, -0.04511},
        {-0.80827, -1.45677, -0.44361, 0.5206},
        {-0.07895, -0.22368, -0.05263, 0.19737},
        {-0.52256, -0.81391, -0.30075, 0.30639},
    });

    EXPECT_TRUE(b == expected);
}

TEST(MatrixTests, TestInverseOfMatrix) {
    commontypes::Matrix a({{8, -5, 9, 2}, {7, 5, 6, 1}, {-6, 0, 9, 6}, {-3, 0, -9, -4}});
    commontypes::Matrix expected({{-0.15385, -0.15385, -0.28205, -0.53846},
                                  {-0.07692, 0.12308, 0.02564, 0.03077},
                                  {0.35897, 0.35897, 0.43590, 0.92308},
                                  {-0.69231, -0.69231, -0.76923, -1.92308}});
    EXPECT_TRUE(a.Inverse() == expected);
}

TEST(MatrixTests, TestInverseOfAnotherMatrix) {
    commontypes::Matrix a({{9, 3, 0, 9}, {-5, -2, -6, -3}, {-4, 9, 6, 4}, {-7, 6, 6, 2}});
    commontypes::Matrix expected({{-0.04074, -0.07778, 0.14444, -0.22222},
                                  {-0.07778, 0.03333, 0.36667, -0.33333},
                                  {-0.02901, -0.14630, -0.10926, 0.12963},
                                  {0.17778, 0.06667, -0.26667, 0.33333}});
    EXPECT_TRUE(a.Inverse() == expected);
}

TEST(MatrixTests, TestMultiplyMatrixByItsInverse) {
    commontypes::Matrix a({{3, -9, 7, 3}, {3, -8, 2, -9}, {-4, 4, 4, 1}, {-6, 5, -1, 1}});
    commontypes::Matrix b({{8, 2, 2, 2}, {3, -1, 7, 0}, {7, 0, 5, 4}, {6, -2, 0, 5}});
    commontypes::Matrix c = a * b;
    ASSERT_TRUE(c * b.Inverse() == a);
}

TEST(MatrixTests, TestMultiplyByATranslationMatrix) {
    commontypes::TranslationMatrix transform{5, -3, 2};
    commontypes::Point p{-3, 4, 5};
    ASSERT_TRUE(transform * p == commontypes::Point(2, 1, 7));
}

TEST(MatrixTests, TestMultiplyByInverseOfTranslationMatrix) {
    commontypes::TranslationMatrix transform{5, -3, 2};
    commontypes::Matrix inverse_translation = transform.Inverse();
    commontypes::Point p{-3, 4, 5};
    ASSERT_TRUE(inverse_translation * p == commontypes::Point(-8, 7, 3));
}

TEST(MatrixTests, TestTranslationDoesNotAffectVectors) {
    commontypes::TranslationMatrix transform{5, -3, 2};
    commontypes::Vector v{-3, 4, 5};
    EXPECT_TRUE(transform * v == v);
}

TEST(MatrixTests, TestScalingMatrixAppliedToAPoint) {
    commontypes::ScalingMatrix transform{2, 3, 4};
    commontypes::Point p{-4, 6, 8};
    ASSERT_TRUE(transform * p == commontypes::Point(-8, 18, 32));
}

TEST(MatrixTests, TestScalingMatrixAppliedToAVector) {
    commontypes::ScalingMatrix transform{2, 3, 4};
    commontypes::Vector v{-4, 6, 8};
    ASSERT_TRUE(transform * v == commontypes::Vector(-8, 18, 32));
}

TEST(MatrixTests, TestMultiplyingByTheInverseOfAScalingMatrix) {
    commontypes::ScalingMatrix transform{2, 3, 4};
    commontypes::Matrix transform_inverse = transform.Inverse();
    commontypes::Vector v{-4, 6, 8};
    ASSERT_TRUE(transform_inverse * v == commontypes::Vector(-2, 2, 2));
}

TEST(MatrixTests, TestReflectionScalingByNegativeValue) {
    commontypes::ScalingMatrix transform{-1, 1, 1};
    commontypes::Point p{2, 3, 4};
    // test reflecting the point across the x axis from positive to negative
    ASSERT_TRUE(transform * p == commontypes::Point(-2, 3, 4));
}

TEST(MatrixTests, TestRotatingPointAroundXaxis) {
    commontypes::Point p{0, 1, 0};
    commontypes::RotationMatrixX half_quarter{M_PI_4};
    commontypes::RotationMatrixX full_quarter{M_PI_2};
    ASSERT_TRUE(half_quarter * p == commontypes::Point(0, sqrt(2) / 2, sqrt(2) / 2));
    ASSERT_TRUE(full_quarter * p == commontypes::Point(0, 0, 1));
}

TEST(MatrixTests, TestInverseOfXrotationRotatesInOppositeDirection) {
    commontypes::Point p{0, 1, 0};
    commontypes::RotationMatrixX half_quarter{M_PI_4};
    commontypes::Matrix inverse = half_quarter.Inverse();
    ASSERT_TRUE(inverse * p == commontypes::Point(0, sqrt(2) / 2, -(sqrt(2) / 2)));
}

TEST(MatrixTests, TestRotatingPointAroundYaxis) {
    commontypes::Point p{0, 0, 1};
    commontypes::RotationMatrixY half_quarter{M_PI_4};
    commontypes::RotationMatrixY full_quarter{M_PI_2};
    ASSERT_TRUE(half_quarter * p == commontypes::Point(sqrt(2) / 2, 0, sqrt(2) / 2));
    ASSERT_TRUE(full_quarter * p == commontypes::Point(1, 0, 0));
}

TEST(MatrixTests, TestRotatingPointAroundZaxis) {
    commontypes::Point p{0, 1, 0};
    commontypes::RotationMatrixZ half_quarter{M_PI_4};
    commontypes::RotationMatrixZ full_quarter{M_PI_2};
    ASSERT_TRUE(half_quarter * p == commontypes::Point(-sqrt(2) / 2, sqrt(2) / 2, 0));
    ASSERT_TRUE(full_quarter * p == commontypes::Point(-1, 0, 0));
}

TEST(MatrixTests, TestShearingTransformationMovesXinProportionToY) {
    commontypes::ShearingMatrix transform{1, 0, 0, 0, 0, 0};
    commontypes::Point p{2, 3, 4};
    ASSERT_TRUE(transform * p == commontypes::Point(5, 3, 4));
}

TEST(MatrixTests, TestShearingTransformationMovesXinProportionToZ) {
    commontypes::ShearingMatrix transform{0, 1, 0, 0, 0, 0};
    commontypes::Point p{2, 3, 4};
    ASSERT_TRUE(transform * p == commontypes::Point(6, 3, 4));
}

TEST(MatrixTests, TestShearingTransformationMovesYinProportionToX) {
    commontypes::ShearingMatrix transform{0, 0, 1, 0, 0, 0};
    commontypes::Point p{2, 3, 4};
    ASSERT_TRUE(transform * p == commontypes::Point(2, 5, 4));
}

TEST(MatrixTests, TestShearingTransformationMovesYinProportionToZ) {
    commontypes::ShearingMatrix transform{0, 0, 0, 1, 0, 0};
    commontypes::Point p{2, 3, 4};
    ASSERT_TRUE(transform * p == commontypes::Point(2, 7, 4));
}

TEST(MatrixTests, TestShearingTransformationMovesZinProportionToX) {
    commontypes::ShearingMatrix transform{0, 0, 0, 0, 1, 0};
    commontypes::Point p{2, 3, 4};
    ASSERT_TRUE(transform * p == commontypes::Point(2, 3, 6));
}

TEST(MatrixTests, TestShearingTransformationMovesZinProportionToY) {
    commontypes::ShearingMatrix transform{0, 0, 0, 0, 0, 1};
    commontypes::Point p{2, 3, 4};
    ASSERT_TRUE(transform * p == commontypes::Point(2, 3, 7));
}

TEST(MatrixTests, TestIndividualTransformationsAreAppliedInSequence) {
    commontypes::Point p{1, 0, 1};
    commontypes::Matrix a = commontypes::RotationMatrixX(M_PI / 2);
    commontypes::Matrix b = commontypes::ScalingMatrix(5, 5, 5);
    commontypes::Matrix c = commontypes::TranslationMatrix(10, 5, 7);

    // apply rotation
    commontypes::Point p2 = commontypes::Point(a * static_cast<commontypes::Tuple>(p));
    ASSERT_TRUE(p2 == commontypes::Point(1, -1, 0));

    // apply scaling
    commontypes::Point p3{b * p2};
    ASSERT_TRUE(p3 == commontypes::Point(5, -5, 0));

    // apply translation
    commontypes::Point p4{c * p3};
    ASSERT_TRUE(p4 == commontypes::Point(15, 0, 7));
}

TEST(MatrixTests, TestChainedTransformationsMustBeAppliedInReverseOrder) {
    commontypes::Point p{1, 0, 1};
    commontypes::Matrix a = commontypes::RotationMatrixX(M_PI_2);
    commontypes::Matrix b = commontypes::ScalingMatrix(5, 5, 5);
    commontypes::Matrix c = commontypes::TranslationMatrix(10, 5, 7);
    commontypes::Matrix t = c * b * a;
    ASSERT_TRUE(commontypes::Point(t * p) == commontypes::Point(15, 0, 7));
}