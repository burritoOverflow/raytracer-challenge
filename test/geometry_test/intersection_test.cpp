#include "intersection.h"
#include <gtest/gtest.h>
#include "plane.h"
#include "scalingmatrix.h"
#include "sphere.h"
#include "translationmatrix.h"

TEST(IntersectionTest, TestIntersectionEncapsulatesTandObject) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Shape> s_ptr = std::make_shared<geometry::Sphere>(s);
    double t{3.5};
    geometry::Intersection i{t, s_ptr};
    ASSERT_EQ(i.t_, t);
    ASSERT_EQ(i.object_, s_ptr);
}

TEST(IntersectionTest, TestAggregatingIntersections) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Shape> s_ptr = std::make_shared<geometry::Sphere>(s);
    geometry::Intersection i1{1, s_ptr};
    geometry::Intersection i2{2, s_ptr};
    std::vector<geometry::Intersection> xs = {i1, i2};

    ASSERT_EQ(xs.size(), 2);
    ASSERT_DOUBLE_EQ(xs.at(0).t_, 1);
    ASSERT_DOUBLE_EQ(xs.at(1).t_, 2);
}

TEST(IntersectionTest, TestIntersectSetsTheObjectOnTheIntersection) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere s{};
    std::shared_ptr<geometry::Shape> s_ptr = std::make_shared<geometry::Sphere>(s);
    const std::vector<geometry::Intersection> xs = s.Intersect(r);

    ASSERT_EQ(xs.size(), 2);
    ASSERT_TRUE(*xs.at(0).object_ == *s_ptr);
    ASSERT_TRUE(*xs.at(1).object_ == *s_ptr);
}

TEST(IntersectionTest, TestHitWhenAllIntersectionsHavePositiveTValues) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Shape> s_ptr = std::make_shared<geometry::Sphere>(s);

    geometry::Intersection i1{1, s_ptr};
    geometry::Intersection i2{2, s_ptr};
    std::vector<geometry::Intersection> xs = {i2, i1};

    std::optional<geometry::Intersection> oi = geometry::Intersection::Hit(xs);
    ASSERT_TRUE(oi.has_value());
    geometry::Intersection i = oi.value();
    ASSERT_TRUE(i == i1);
}

TEST(IntersectionTest, TestHitWhenSomeIntersectionsHaveNegativeTValues) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Shape> s_ptr = std::make_shared<geometry::Sphere>(s);

    geometry::Intersection i1{-1, s_ptr};
    geometry::Intersection i2{1, s_ptr};
    std::vector<geometry::Intersection> xs = {i2, i1};

    std::optional<geometry::Intersection> oi = geometry::Intersection::Hit(xs);
    ASSERT_TRUE(oi.has_value());
    geometry::Intersection i = oi.value();
    ASSERT_TRUE(i == i2);
}

TEST(IntersectionTest, TestHitWhenAllIntersectionsHaveNegativeTValues) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Shape> s_ptr = std::make_shared<geometry::Sphere>(s);

    geometry::Intersection i1{-2, s_ptr};
    geometry::Intersection i2{-1, s_ptr};
    std::vector<geometry::Intersection> xs = {i2, i1};

    // no hits should be present when t values are all negative
    std::optional<geometry::Intersection> oi = geometry::Intersection::Hit(xs);
    ASSERT_FALSE(oi.has_value());
}

TEST(IntersectionTest, TestHitIsAlwaysTheLowestNonnegativeIntersection) {
    geometry::Sphere s{};
    std::shared_ptr<geometry::Shape> s_ptr = std::make_shared<geometry::Sphere>(s);

    geometry::Intersection i1{5, s_ptr};
    geometry::Intersection i2{7, s_ptr};
    geometry::Intersection i3{-3, s_ptr};
    geometry::Intersection i4{2, s_ptr};
    std::vector<geometry::Intersection> xs{i1, i2, i3, i4};

    std::optional<geometry::Intersection> oi = geometry::Intersection::Hit(xs);
    ASSERT_TRUE(oi.has_value());
    ASSERT_TRUE(oi.value() == i4);
}

TEST(IntersectionTest, TestPrecomputingStateOfIntersection) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere shape;
    geometry::Intersection i{4, std::make_shared<geometry::Sphere>(shape)};
    geometry::Computations comps = i.PrepareComputations(r);
    ASSERT_TRUE(comps.t_ == i.t_);
    ASSERT_TRUE(*comps.object_ == *i.object_);
    ASSERT_TRUE(comps.point_ == commontypes::Point(0, 0, -1));
    ASSERT_TRUE(comps.eye_vector_ == commontypes::Vector(0, 0, -1));
    ASSERT_TRUE(comps.normal_vector_ == commontypes::Vector(0, 0, -1));
}

TEST(IntersectionTest, TestHitWhenIntersectionOccursOnOutside) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere shape{};
    geometry::Intersection i{4, std::make_shared<geometry::Sphere>(shape)};
    geometry::Computations comps = i.PrepareComputations(r);
    ASSERT_FALSE(comps.inside_);
}

TEST(IntersectionTest, TestHitWhenIntersectionOccursOnInside) {
    commontypes::Ray r{commontypes::Point{0, 0, 0}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere shape{};
    geometry::Intersection i{1, std::make_shared<geometry::Sphere>(shape)};
    geometry::Computations comps = i.PrepareComputations(r);

    ASSERT_TRUE(comps.point_ == commontypes::Point(0, 0, 1));
    ASSERT_TRUE(comps.eye_vector_ == commontypes::Vector(0, 0, -1));
    ASSERT_TRUE(comps.inside_);
    ASSERT_TRUE(comps.normal_vector_ == commontypes::Vector(0, 0, -1));
}

TEST(IntersectionTest, TestHitShouldOffsetThePoint) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere s{};
    s.SetTransform(commontypes::TranslationMatrix{0, 0, 1});

    const geometry::Intersection i{5, std::make_shared<geometry::Sphere>(s)};
    const geometry::Computations comps = i.PrepareComputations(r);

    // check that the point has been adjusted in the correct direction
    ASSERT_TRUE(comps.over_point_.z() < -utility::EPSILON_ / 2);
    ASSERT_TRUE(comps.point_.z() > comps.over_point_.z());
}

TEST(IntersectionTest, TestPrecomputingTheReflectionVector) {
    geometry::Plane shape{};
    const double sqrt_2_over_2 = sqrt(2) / 2;
    commontypes::Ray r{commontypes::Point{0, 1, -1},
                       commontypes::Vector{0, -sqrt_2_over_2, sqrt_2_over_2}};

    const geometry::Intersection i{sqrt(2), std::make_shared<geometry::Plane>(shape)};
    const geometry::Computations comps = i.PrepareComputations(r);
    // see pg. 143
    ASSERT_TRUE(comps.reflect_vector_ == commontypes::Vector(0, sqrt_2_over_2, sqrt_2_over_2));
}

// pg. 152
TEST(IntersectionTest, TestFindingN1AndN2AtVariousIntersection) {
    auto a = std::make_shared<geometry::Sphere>(geometry::Sphere::GlassSphere());
    a->SetTransform(commontypes::ScalingMatrix{2, 2, 2});
    a->SetMaterial(std::make_shared<lighting::Material>(
        lighting::MaterialBuilder().WithRefractiveIndex(1.5)));

    auto b = std::make_shared<geometry::Sphere>(geometry::Sphere::GlassSphere());
    b->SetTransform(commontypes::TranslationMatrix{0, 0, -0.25});
    b->SetMaterial(std::make_shared<lighting::Material>(
        lighting::MaterialBuilder().WithRefractiveIndex(2.0)));

    auto c = std::make_shared<geometry::Sphere>(geometry::Sphere::GlassSphere());
    c->SetTransform(commontypes::TranslationMatrix{0, 0, 0.25});
    c->SetMaterial(std::make_shared<lighting::Material>(
        lighting::MaterialBuilder().WithRefractiveIndex(2.5)));

    auto r = commontypes::Ray{commontypes::Point{0, 0, -4}, commontypes::Vector{0, 0, 1}};
    const auto xs = std::vector<geometry::Intersection>{geometry::Intersection{2, a},
                                                        geometry::Intersection{2.75, b},
                                                        geometry::Intersection{3.25, c},
                                                        geometry::Intersection{4.75, b},
                                                        geometry::Intersection{5.25, c},
                                                        geometry::Intersection{6, a}

    };

    struct ExpectedValues {
        const double n1;
        const double n2;
    };

    const std::vector<ExpectedValues> expected_vals{{1.0, 1.5}, {1.5, 2.0}, {2.0, 2.5},
                                                    {2.5, 2.5}, {2.5, 1.5}, {1.5, 1.0}};

    for (size_t i = 0; i < expected_vals.size(); ++i) {
        const auto comps = xs.at(i).PrepareComputations(r, xs);
        const auto expected = expected_vals.at(i);

        ASSERT_DOUBLE_EQ(comps.n1, expected.n1);
        ASSERT_DOUBLE_EQ(comps.n2, expected.n2);
    }
}

TEST(IntersectionTest, TestUnderPointIsOffsetBelowSurface) {
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    geometry::Sphere shape = geometry::Sphere::GlassSphere();
    shape.SetTransform(commontypes::TranslationMatrix{0, 0, 1});

    const geometry::Intersection i{5, std::make_shared<geometry::Sphere>(shape)};
    const auto xs = std::vector<geometry::Intersection>{i};

    const auto comps = i.PrepareComputations(r, xs);
    ASSERT_GT(comps.under_point_.z(), utility::EPSILON_ / 2);
    ASSERT_LT(comps.point_.z(), comps.under_point_.z());
}

TEST(IntersectionTest, TestSchlickApproximationUnderTotalInternalReflection) {
    geometry::Sphere shape = geometry::Sphere::GlassSphere();
    auto shape_ptr = std::make_shared<geometry::Sphere>(shape);

    const double sqrt_2_over_2 = sqrt(2) / 2;

    // ray inside a glass sphere, offset from center straight up
    commontypes::Ray r{commontypes::Point{0, 0, sqrt_2_over_2}, commontypes::Vector{0, 1, 0}};
    const auto xs =
        std::vector<geometry::Intersection>{geometry::Intersection{-sqrt_2_over_2, shape_ptr},
                                            geometry::Intersection{sqrt_2_over_2, shape_ptr}};

    const auto comps = xs.at(1).PrepareComputations(r, xs);
    const double reflectance = geometry::Schlick(comps);
    ASSERT_DOUBLE_EQ(reflectance, 1.0);
}

TEST(IntersectionTest, TestSchlickApproximationWithPerpendicularViewingAngle) {
    geometry::Sphere shape = geometry::Sphere::GlassSphere();
    auto shape_ptr = std::make_shared<geometry::Sphere>(shape);

    commontypes::Ray r{commontypes::Point{0, 0, 0}, commontypes::Vector{0, 1, 0}};
    const auto xs = std::vector<geometry::Intersection>{geometry::Intersection{-1, shape_ptr},
                                                        geometry::Intersection{1, shape_ptr}};

    const auto comps = xs.at(1).PrepareComputations(r, xs);
    const double reflectance = geometry::Schlick(comps);
    ASSERT_DOUBLE_EQ(reflectance, 0.04);
}

TEST(IntersectionTest, TestSchlickApproximationWithSmallAngleAndN2GreaterThanN1) {
    geometry::Sphere shape = geometry::Sphere::GlassSphere();
    auto shape_ptr = std::make_shared<geometry::Sphere>(shape);

    commontypes::Ray r{commontypes::Point{0, 0.99, -2}, commontypes::Vector{0, 0, 1}};
    const auto xs = std::vector<geometry::Intersection>{geometry::Intersection{1.8589, shape_ptr}};

    const auto comps = xs.at(0).PrepareComputations(r, xs);
    const double reflectance = geometry::Schlick(comps);

    // expand this value quite a bit to get eq to pass
    ASSERT_DOUBLE_EQ(reflectance, 0.48873081012212183);
}
