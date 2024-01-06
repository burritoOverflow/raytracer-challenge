#include "shape.h"
#include <gtest/gtest.h>
#include <memory>
#include "identitymatrix.h"
#include "material.h"
#include "rotationmatrix.h"
#include "scalingmatrix.h"
#include "translationmatrix.h"

namespace geometry {
class TestShape : public Shape {
   public:
    TestShape() : Shape(), saved_ray_{commontypes::Point{}, commontypes::Vector{}} {};

    std::vector<Intersection> LocalIntersect(const commontypes::Ray& ray) const override {
        saved_ray_ = ray;  // for the purposes of the tests--from pg. 119-120
        return {};
    }

    commontypes::Vector LocalNormalAt(const commontypes::Point& local_point) const override {
        // per pg. 121
        return commontypes::Vector{local_point.x(), local_point.y(), local_point.z()};
    }

    // see pg. 119-120
    mutable commontypes::Ray
        saved_ray_;  // we actually do want to mutate this to verify the behavior of `Shape`
};
}  // namespace geometry

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