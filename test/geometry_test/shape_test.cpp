#include "shape.h"
#include <gtest/gtest.h>
#include <memory>
#include "identitymatrix.h"
#include "material.h"
#include "scalingmatrix.h"
#include "translationmatrix.h"

TEST(ShapeTest, TestTheDefaultTransformation) {
    geometry::TestShape s{};
    ASSERT_TRUE(s.transform() == commontypes::IdentityMatrix());
}

TEST(ShapeTest, TestAssigningATransformation) {
    geometry::TestShape s{};
    s.SetTransform(commontypes::TranslationMatrix{2, 3, 4});
    ASSERT_TRUE(s.transform() == commontypes::TranslationMatrix(2, 3, 4));
}

TEST(ShapeTest, TestTheDefaultMaterial) {
    geometry::TestShape s{};
    auto m = s.material();
    ASSERT_TRUE(*m == lighting::Material());
}

TEST(ShapeTest, TestAssigningAMaterial) {
    geometry::TestShape s{};
    lighting::Material m = lighting::Material{};
    m.SetAmbient(1);
    s.SetMaterial(std::make_shared<lighting::Material>(m));
    ASSERT_TRUE(*s.material() == m);
}

TEST(ShapeTest, TestIntersectingScaledShapeWithRay) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::TestShape s{};
    s.SetTransform(commontypes::ScalingMatrix{2, 2, 2});
    std::vector<geometry::Intersection> xs = s.Intersect(r);
    ASSERT_TRUE(s.saved_ray_.origin() == commontypes::Point(0, 0, -2.5));
    ASSERT_TRUE(s.saved_ray_.direction() == commontypes::Vector(0, 0, 0.5));
}

TEST(ShapeTest, TestIntersectingTranslatedShapeWithRay) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::TestShape s{};
    s.SetTransform(commontypes::TranslationMatrix{5, 0, 0});
    std::vector<geometry::Intersection> xs = s.Intersect(r);
    ASSERT_TRUE(s.saved_ray_.origin() == commontypes::Point(-5, 0, -5));
    ASSERT_TRUE(s.saved_ray_.direction() == commontypes::Vector(0, 0, 1));
}
