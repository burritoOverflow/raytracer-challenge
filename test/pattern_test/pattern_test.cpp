#include "pattern.h"
#include <gtest/gtest.h>
#include "color.h"
#include "identitymatrix.h"
#include "point.h"
#include "scalingmatrix.h"
#include "sphere.h"
#include "test_classes.h"
#include "translationmatrix.h"

TEST(PatternTest, TestTheDefaultPatternTransformation) {
    const pattern::TestPattern pattern{};
    ASSERT_TRUE(pattern.GetPatternTransform() == commontypes::IdentityMatrix());
}

TEST(PatternTest, TestAssigningTransformation) {
    pattern::TestPattern pattern{};
    pattern.SetPatternTransform(commontypes::TranslationMatrix{1, 2, 3});
    ASSERT_TRUE(pattern.GetPatternTransform() == commontypes::TranslationMatrix(1, 2, 3));
}

TEST(PatternTest, TestPatternWithObjectTransformation) {
    geometry::Sphere shape{};
    shape.SetTransform(commontypes::ScalingMatrix{2, 2, 2});
    const pattern::TestPattern pattern{};
    const commontypes::Color c =
        pattern.PatternAtShape(shape.GetTransform(), commontypes::Point{2, 3, 4});
    ASSERT_TRUE(c == commontypes::Color(1, 1.5, 2));
}

TEST(PatternTest, TestPatternWithPatternTransformation) {
    geometry::Sphere shape{};
    pattern::TestPattern pattern{};
    pattern.SetPatternTransform(commontypes::ScalingMatrix{2, 2, 2});
    const commontypes::Color c =
        pattern.PatternAtShape(shape.GetTransform(), commontypes::Point{2, 3, 4});
    ASSERT_TRUE(c == commontypes::Color(1, 1.5, 2));
}

TEST(PatternTest, TestPatternWithBothObjectAndPatternTransformation) {
    geometry::Sphere shape{};
    shape.SetTransform(commontypes::ScalingMatrix{2, 2, 2});
    pattern::TestPattern pattern{};
    pattern.SetPatternTransform(commontypes::TranslationMatrix{0.5, 1, 1.5});
    const commontypes::Color c =
        pattern.PatternAtShape(shape.GetTransform(), commontypes::Point{2.5, 3, 3.5});
    ASSERT_TRUE(c == commontypes::Color(0.75, 0.5, 0.25));
}