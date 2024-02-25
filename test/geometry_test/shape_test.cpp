#include "shape.h"
#include <gtest/gtest.h>
#include <memory>
#include "group.h"
#include "identitymatrix.h"
#include "material.h"
#include "rotationmatrix.h"
#include "scalingmatrix.h"
#include "sphere.h"
#include "test_classes.h"
#include "translationmatrix.h"

TEST(ShapeTest, TestEachShapeHasUniqueId) {
    std::set<uint64_t> shape_id_set{};
    const size_t n_elems{1000};

    for (int i = 0; i < n_elems; ++i) {
        geometry::TestShape shape{};
        uint64_t shape_id = shape.id();
        auto result = shape_id_set.insert(shape_id);
        ASSERT_TRUE(result.second);
    }

    ASSERT_EQ(shape_id_set.size(), n_elems);
}

TEST(ShapeTest, TestTheDefaultTransformation) {
    geometry::TestShape s{};
    ASSERT_TRUE(s.Transform() == commontypes::IdentityMatrix());
}

TEST(ShapeTest, TestAssigningATransformation) {
    geometry::TestShape s{};
    s.SetTransform(commontypes::TranslationMatrix{2, 3, 4});
    ASSERT_TRUE(s.Transform() == commontypes::TranslationMatrix(2, 3, 4));
}

TEST(ShapeTest, TestTheDefaultMaterial) {
    geometry::TestShape s{};
    auto m = s.Material();
    ASSERT_TRUE(*m == lighting::Material());
}

TEST(ShapeTest, TestAssigningAMaterial) {
    geometry::TestShape s{};
    lighting::Material m = lighting::Material{};
    m.SetAmbient(1);
    s.SetMaterial(std::make_shared<lighting::Material>(m));
    ASSERT_TRUE(*s.Material() == m);
}

TEST(ShapeTest, TestIntersectingScaledShapeWithRay) {
    const commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::TestShape s{};
    s.SetTransform(commontypes::ScalingMatrix{2, 2, 2});
    const std::vector<geometry::Intersection> xs = s.Intersect(r);
    ASSERT_TRUE(s.saved_ray_.origin() == commontypes::Point(0, 0, -2.5));
    ASSERT_TRUE(s.saved_ray_.direction() == commontypes::Vector(0, 0, 0.5));
}

TEST(ShapeTest, TestIntersectingTranslatedShapeWithRay) {
    const commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::TestShape s{};
    s.SetTransform(commontypes::TranslationMatrix{5, 0, 0});
    const std::vector<geometry::Intersection> xs = s.Intersect(r);
    ASSERT_TRUE(s.saved_ray_.origin() == commontypes::Point(-5, 0, -5));
    ASSERT_TRUE(s.saved_ray_.direction() == commontypes::Vector(0, 0, 1));
}

// the following two tests demonstrate that translation does not effect the normal
// but that scaling and rotation do (pg. 121)
TEST(ShapeTest, TestComputingNormalOnTranslatedShape) {
    geometry::TestShape s{};
    s.SetTransform(commontypes::TranslationMatrix{0, 1, 0});
    const commontypes::Vector n = s.NormalAt(commontypes::Point{0, 1.70711, -0.70711});
    ASSERT_TRUE(n == commontypes::Vector(0, 0.70711, -0.70711));
}

TEST(ShapeTest, TestComputingNormalOnATransformedShape) {
    geometry::TestShape s{};
    commontypes::Matrix m =
        commontypes::ScalingMatrix{1, 0.5, 1} * commontypes::RotationMatrixZ{M_PI / 5};
    s.SetTransform(m);
    const double sqrt2_over2 = sqrt(2) / 2;
    const commontypes::Vector n = s.NormalAt(commontypes::Point{0, sqrt2_over2, -sqrt2_over2});
    ASSERT_TRUE(n == commontypes::Vector(0, 0.97014, -0.24254));
}

TEST(ShapeTest, TestConvertingPointFromWorldToObjectSpace) {
    geometry::Group g1{};
    g1.SetTransform(commontypes::RotationMatrixY{M_PI_2});

    std::shared_ptr<geometry::Shape> group2_ptr = std::make_shared<geometry::Group>();
    group2_ptr->SetTransform(commontypes::ScalingMatrix{2, 2, 2});
    g1.AddChildToGroup(group2_ptr);

    std::shared_ptr<geometry::Shape> sphere_ptr = std::make_shared<geometry::Sphere>();
    sphere_ptr->SetTransform(commontypes::TranslationMatrix{5, 0, 0});

    // TODO this is a mess and the design should be reconsidered to avoid all of this.
    dynamic_cast<geometry::Group*>(group2_ptr.get())->AddChildToGroup(sphere_ptr);

    const auto p = dynamic_cast<geometry::Sphere*>(sphere_ptr.get())
                       ->WorldToObject(commontypes::Point{-2, 0, -10});

    ASSERT_TRUE(p == commontypes::Point(0, 0, -1));
}

TEST(ShapeTest, TestConvertingNormalFromObjectToWorldSpace) {
    geometry::Group g1{};
    g1.SetTransform(commontypes::RotationMatrixY{M_PI_2});

    std::shared_ptr<geometry::Shape> group2_ptr = std::make_shared<geometry::Group>();
    group2_ptr->SetTransform(commontypes::ScalingMatrix{1, 2, 3});
    g1.AddChildToGroup(group2_ptr);

    std::shared_ptr<geometry::Shape> sphere_ptr = std::make_shared<geometry::Sphere>();
    sphere_ptr->SetTransform(commontypes::TranslationMatrix{5, 0, 0});

    dynamic_cast<geometry::Group*>(group2_ptr.get())->AddChildToGroup(sphere_ptr);
    const double sqrt_3_over_3 = sqrt(3) / 3;

    const auto n =
        dynamic_cast<geometry::Sphere*>(sphere_ptr.get())
            ->NormalToWorld(commontypes::Vector{sqrt_3_over_3, sqrt_3_over_3, sqrt_3_over_3});

    ASSERT_TRUE(n == commontypes::Vector(0.2857, 0.4286, -0.8571));
}

TEST(ShapeTest, TestFindingTheNormalOnChildObject) {
    geometry::Group g1{};
    g1.SetTransform(commontypes::RotationMatrixY{M_PI_2});

    std::shared_ptr<geometry::Shape> group2_ptr = std::make_shared<geometry::Group>();
    group2_ptr->SetTransform(commontypes::ScalingMatrix{1, 2, 3});
    g1.AddChildToGroup(group2_ptr);

    std::shared_ptr<geometry::Shape> sphere_ptr = std::make_shared<geometry::Sphere>();
    sphere_ptr->SetTransform(commontypes::TranslationMatrix{5, 0, 0});

    dynamic_cast<geometry::Group*>(group2_ptr.get())->AddChildToGroup(sphere_ptr);

    // check that the normal calculated on this Group's child is as expected
    const auto n = dynamic_cast<geometry::Sphere*>(sphere_ptr.get())
                       ->NormalAt(commontypes::Point{1.7321, 1.1547, -5.5774});

    ASSERT_TRUE(n == commontypes::Vector(0.2857, 0.4286, -0.8571));
}
