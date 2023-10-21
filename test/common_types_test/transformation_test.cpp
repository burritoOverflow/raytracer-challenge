#include <gtest/gtest.h>
#include "identitymatrix.h"
#include "point.h"
#include "scalingmatrix.h"
#include "utility.h"
#include "viewtransform.h"

TEST(TransformationTest, TestTransformationMatrixForDefaultOrientation) {
    commontypes::Point from{0, 0, 0};
    commontypes::Point to{0, 0, -1};
    commontypes::Vector up{0, 1, 0};
    commontypes::ViewTransform t{from, to, up};
    ASSERT_EQ(t.n_rows(), 4);
    ASSERT_EQ(t.n_columns(), 4);
    ASSERT_TRUE(t == commontypes::IdentityMatrix());
}

TEST(TransformationTest, TestTransformationMatrixLookingInPositiveZDirection) {
    commontypes::Point from{0, 0, 0};
    commontypes::Point to{0, 0, 1};
    commontypes::Vector up{0, 1, 0};
    commontypes::ViewTransform t{from, to, up};
    ASSERT_EQ(t.n_rows(), 4);
    ASSERT_EQ(t.n_columns(), 4);
    ASSERT_TRUE(t == commontypes::ScalingMatrix(-1, 1, -1));
}

TEST(TransformationTest, TestViewTransformationMovesTheWorld) {
    commontypes::Point from{0, 0, 8};
    commontypes::Point to{0, 0, 0};
    commontypes::Vector up{0, 1, 0};
    commontypes::ViewTransform t{from, to, up};
    ASSERT_EQ(t.n_rows(), 4);
    ASSERT_EQ(t.n_columns(), 4);
    ASSERT_TRUE(t == commontypes::TranslationMatrix(0, 0, -8));
}

TEST(TransformationTest, TestArbitraryViewTransformation) {
    commontypes::Point from{1, 3, 2};
    commontypes::Point to{4, -2, 8};
    commontypes::Vector up{1, 1, 0};
    commontypes::ViewTransform t{from, to, up};
    
    ASSERT_EQ(t.n_rows(), 4);
    ASSERT_EQ(t.n_columns(), 4);

    std::vector<double> first{-0.50709, 0.50709, 0.67612, -2.36643};
    std::vector<double> second{0.76772, 0.60609, 0.12122, -2.82843};
    std::vector<double> third{-0.35857, 0.59761, -0.71714, 0.0};
    std::vector<double> fourth{0.0, 0.0, 0.0, 1.0};

    std::vector<std::vector<double>> expected_v;
    expected_v.emplace_back(first);
    expected_v.emplace_back(second);
    expected_v.emplace_back(third);
    expected_v.emplace_back(fourth);
    const size_t NUM_COLS = 4;

    for (size_t row_idx = 0; row_idx < expected_v.size(); ++row_idx) {
        for (size_t col_idx = 0; col_idx < NUM_COLS; ++col_idx) {
            const double expected = expected_v.at(row_idx).at(col_idx);
            const double actual = t.matrix().at(row_idx).at(col_idx);
            // close enough for our purposes
            ASSERT_TRUE(utility::NearEquals(expected, actual));
        }
    }
}