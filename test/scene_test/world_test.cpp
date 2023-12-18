#include "world.h"
#include <gtest/gtest.h>
#include "pattern.h"
#include "plane.h"
#include "scalingmatrix.h"
#include "translationmatrix.h"

static const double SQRT2OVER2 = sqrt(2) / 2;

namespace {
class TestPattern : public pattern::Pattern {
   public:
    TestPattern() : pattern::Pattern() {}

    const commontypes::Color PatternAt(const commontypes::Point& point) const override {
        return {point.x(), point.y(), point.z()};
    }
};
}  // namespace

// workaround for testing for the presence of the expected Spheres for the default world
static bool DefaultWorldContains(const scene::World& default_world,
                                 const std::shared_ptr<geometry::Sphere>& sphere_ptr) {
    const auto default_world_objects = default_world.objects();
    return std::any_of(default_world_objects.begin(), default_world_objects.end(),
                       [&sphere_ptr](const auto& default_world_obj) {
                           const auto world_sphere =
                               dynamic_cast<geometry::Sphere*>(default_world_obj.get());
                           return *world_sphere == *sphere_ptr;
                       });
}

TEST(WorldTest, TestCreatingWorld) {
    scene::World world{};
    ASSERT_TRUE(world.light() == nullptr);
    ASSERT_TRUE(world.objects().empty());
}

TEST(WorldTest, TestAddObjectsToWorld) {
    scene::World world{};
    geometry::Sphere s1{};
    geometry::Sphere s2{};
    geometry::Sphere s3{};
    geometry::Sphere s4{};

    const auto s1_ptr = std::make_shared<geometry::Sphere>(s1);
    const auto s2_ptr = std::make_shared<geometry::Sphere>(s2);
    std::vector<std::shared_ptr<geometry::Shape>> v1{s1_ptr, s2_ptr};

    world.AddObjects(std::move(v1));
    ASSERT_EQ(world.objects().size(), 2);
    ASSERT_TRUE(world.WorldContains(s1_ptr));
    ASSERT_TRUE(world.WorldContains(s2_ptr));

    const auto s3_ptr = std::make_shared<geometry::Sphere>(s3);
    const auto s4_ptr = std::make_shared<geometry::Sphere>(s4);
    std::vector<std::shared_ptr<geometry::Shape>> v2{s3_ptr, s4_ptr};

    world.AddObjects(std::move(v2));
    ASSERT_EQ(world.objects().size(), 4);
    ASSERT_TRUE(world.WorldContains(s3_ptr));
    ASSERT_TRUE(world.WorldContains(s4_ptr));
}

TEST(WorldTest, TestDefaultWorld) {
    const scene::World default_world = scene::World::DefaultWorld();

    const lighting::PointLight light{commontypes::Point{-10, 10, -10},
                                     commontypes::Color{1, 1, 1}};

    geometry::Sphere s1;

    lighting::Material material;
    material.SetColor(commontypes::Color{0.8, 1.0, 0.6});
    material.SetDiffuse(0.7);
    material.SetSpecular(0.2);

    s1.SetMaterial(std::make_shared<lighting::Material>(material));

    geometry::Sphere s2;
    s2.SetTransform(commontypes::ScalingMatrix{0.5, 0.5, 0.5});

    const auto world_light = *default_world.light();
    ASSERT_TRUE(world_light == light);

    // the default operator for the Shape base class compares by the object's id
    // we cannot have the same id here considering we aren't instantiating the default_world's
    // Shapes, so we'll fall back to this approach that uses the derived Sphere class' == operator
    ASSERT_TRUE(DefaultWorldContains(default_world, std::make_shared<geometry::Sphere>(s1)));
    ASSERT_TRUE(DefaultWorldContains(default_world, std::make_shared<geometry::Sphere>(s2)));
}

TEST(WorldTest, TestIntersectWorldWithRay) {
    scene::World default_world = scene::World::DefaultWorld();
    commontypes::Ray ray{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    std::vector<geometry::Intersection> xs = default_world.Intersect(ray);

    ASSERT_TRUE(xs.size() == 4);
    ASSERT_DOUBLE_EQ(xs.at(0).t_, 4);
    ASSERT_DOUBLE_EQ(xs.at(1).t_, 4.5);
    ASSERT_DOUBLE_EQ(xs.at(2).t_, 5.5);
    ASSERT_DOUBLE_EQ(xs.at(3).t_, 6);
}

TEST(WorldTest, TestShadingAnIntersection) {
    scene::World default_world = scene::World::DefaultWorld();
    commontypes::Ray r{{0, 0, -5}, {0, 0, 1}};
    auto shape = default_world.objects().at(0);
    geometry::Intersection i{4, shape};
    geometry::Computations comps = i.PrepareComputations(r);
    const commontypes::Color c = default_world.ShadeHit(comps);
    ASSERT_TRUE(c == commontypes::Color(0.38066, 0.47583, 0.2855));
}

TEST(WorldTest, TestShadingAnIntersectionFromInside) {
    scene::World default_world = scene::World::DefaultWorld();
    lighting::PointLight point_light{{0, 0.25, 0}, {1, 1, 1}};
    default_world.SetLight(std::make_shared<lighting::PointLight>(point_light));
    commontypes::Ray r{{0, 0, 0}, {0, 0, 1}};
    auto shape = default_world.objects().at(1);
    geometry::Intersection i{0.5, shape};
    geometry::Computations comps = i.PrepareComputations(r);
    const commontypes::Color c = default_world.ShadeHit(comps);
    ASSERT_TRUE(c == commontypes::Color(0.90498, 0.90498, 0.90498));
}

TEST(WorldTest, TestColorWhenRayMisses) {
    scene::World default_world = scene::World::DefaultWorld();
    commontypes::Ray r{{0, 0, -5}, {0, 1, 0}};
    const commontypes::Color c = default_world.ColorAt(r);
    ASSERT_TRUE(c == commontypes::Color(0, 0, 0));
}

TEST(WorldTest, TestColorWhenRayHits) {
    scene::World default_world = scene::World::DefaultWorld();
    commontypes::Ray r{{0, 0, -5}, {0, 0, 1}};
    const commontypes::Color c = default_world.ColorAt(r);
    ASSERT_TRUE(c == commontypes::Color(0.38066, 0.47583, 0.2855));
}

TEST(WorldTest, TestColorWhenIntersectionBehindRay) {
    scene::World default_world = scene::World::DefaultWorld();

    auto outer = default_world.objects().at(0);
    outer->Material()->SetAmbient(1);

    auto inner = default_world.objects().at(1);
    inner->Material()->SetAmbient(1);

    commontypes::Ray r{{0, 0, 0.75}, {0, 0, -1}};
    const commontypes::Color c = default_world.ColorAt(r);
    ASSERT_TRUE(c == inner->Material()->Color());
}

TEST(WorldTest, TestNoShadowWhenNothingIsCollinearWithPointAndLight) {
    scene::World w = scene::World::DefaultWorld();
    const commontypes::Point p{0, 10, 0};
    const auto is_shadowed = w.IsShadowed(p);
    EXPECT_FALSE(is_shadowed);
}

TEST(WorldTest, TestTheShadowWhenObjectIsBetweenPointAndLight) {
    scene::World w = scene::World::DefaultWorld();
    const commontypes::Point p{10, -10, 10};
    const auto is_shadowed = w.IsShadowed(p);
    EXPECT_TRUE(is_shadowed);
}

TEST(WorldTest, TestThereIsNoShadowWhenObjectIsBehindTheLight) {
    scene::World w = scene::World::DefaultWorld();
    const commontypes::Point p{-20, 20, -20};
    const auto is_shadowed = w.IsShadowed(p);
    EXPECT_FALSE(is_shadowed);
}

TEST(WorldTest, TestThereIsNoShadowWhenObjectIsBehindThePoint) {
    scene::World w = scene::World::DefaultWorld();
    const commontypes::Point p{-2, 2, -2};
    const auto is_shadowed = w.IsShadowed(p);
    EXPECT_FALSE(is_shadowed);
}

TEST(WorldTest, TestShadeHitIsGivenAnIntersectionInShadow) {
    scene::World w{};
    lighting::PointLight point_light{{0, 0, -10}, {1, 1, 1}};
    w.SetLight(std::make_shared<lighting::PointLight>(point_light));

    auto s1 = std::make_shared<geometry::Sphere>(geometry::Sphere{});
    auto s2 = std::make_shared<geometry::Sphere>(geometry::Sphere{});
    s2->SetTransform(commontypes::TranslationMatrix{0, 0, 10});

    w.AddObjects(std::vector<std::shared_ptr<geometry::Shape>>{s1, s2});
    commontypes::Ray r{{0, 0, 5}, {0, 0, 1}};
    geometry::Intersection i{4, s2};
    auto comps = i.PrepareComputations(r);
    commontypes::Color c = w.ShadeHit(comps);

    ASSERT_TRUE(c == commontypes::Color(0.1, 0.1, 0.1));
}

TEST(WorldTest, TestReflectedColorForNonreflectiveMaterial) {
    scene::World w = scene::World::DefaultWorld();
    commontypes::Ray r{commontypes::Point{0, 0, 0}, commontypes::Vector{0, 0, 1}};
    auto shape = w.objects().at(1);
    auto shape_material = shape->Material();
    shape_material->SetAmbient(1);

    geometry::Intersection i{1, shape};
    auto comps = i.PrepareComputations(r);
    const commontypes::Color color = w.ReflectedColor(comps);

    ASSERT_TRUE(color == commontypes::Color(0, 0, 0));
}

TEST(WorldTest, TestReflectedColorForReflectiveMaterial) {
    scene::World w = scene::World::DefaultWorld();
    geometry::Plane shape{};

    lighting::Material shape_mat{};
    shape_mat.SetReflective(0.5);

    const commontypes::TranslationMatrix translation{0, -1, 0};
    shape.SetTransform(translation);
    shape.SetMaterial(std::make_shared<lighting::Material>(shape_mat));

    auto shape_ptr = std::make_shared<geometry::Plane>(shape);
    w.AddObject(shape_ptr);

    commontypes::Ray r{commontypes::Point{0, 0, -3},
                       commontypes::Vector{0, -SQRT2OVER2, SQRT2OVER2}};
    geometry::Intersection i{sqrt(2), shape_ptr};
    auto comps = i.PrepareComputations(r);

    const commontypes::Color reflected_color = w.ReflectedColor(comps);
    ASSERT_TRUE(reflected_color == commontypes::Color(0.19032, 0.2379, 0.14274));
}

TEST(WorldTest, TestShadeHitWithReflectedMaterial) {
    scene::World w = scene::World::DefaultWorld();
    geometry::Plane shape{};

    lighting::Material shape_mat{};
    shape_mat.SetReflective(0.5);

    const commontypes::TranslationMatrix translation{0, -1, 0};
    shape.SetTransform(translation);
    shape.SetMaterial(std::make_shared<lighting::Material>(shape_mat));

    auto shape_ptr = std::make_shared<geometry::Plane>(shape);
    w.AddObject(shape_ptr);

    commontypes::Ray r{commontypes::Point{0, 0, -3},
                       commontypes::Vector{0, -SQRT2OVER2, SQRT2OVER2}};
    geometry::Intersection i{sqrt(2), shape_ptr};

    auto comps = i.PrepareComputations(r);
    const commontypes::Color color = w.ShadeHit(comps);
    ASSERT_TRUE(color == commontypes::Color(0.87677, 0.92436, 0.82918));
}

TEST(WorldTest, TestColorAtWithMutuallyReflectiveSurfaces) {
    scene::World w{};
    lighting::PointLight light{commontypes::Point{0, 0, 0}, commontypes::Color{1, 1, 1}};
    w.SetLight(std::make_shared<lighting::PointLight>(light));

    auto material = lighting::Material{};
    material.SetReflective(1);
    const auto mat_ptr = std::make_shared<lighting::Material>(material);

    geometry::Plane lower{};
    lower.SetMaterial(mat_ptr);
    lower.SetTransform(commontypes::TranslationMatrix{0, -1, 0});
    w.AddObject(std::make_shared<geometry::Plane>(lower));

    auto upper_mat = lighting::Material{};
    geometry::Plane upper{};
    upper.SetMaterial(mat_ptr);
    upper.SetTransform(commontypes::TranslationMatrix{0, 1, 0});
    w.AddObject(std::make_shared<geometry::Plane>(upper));

    // no assertions are present, as we are merely ensuring the recursion limit is effective in
    // preventing infinite recursion
    commontypes::Ray r{commontypes::Point{
                           0,
                           0,
                           0,
                       },
                       commontypes::Vector{0, 1, 0}};
    w.ColorAt(r);
}

TEST(WorldTest, TestReflectedColorAtMaxRecursionDepth) {
    auto w = scene::World::DefaultWorld();

    auto shape = geometry::Plane{};

    auto plane_mat = lighting::Material{};
    plane_mat.SetReflective(0.5);

    shape.SetMaterial(std::make_shared<lighting::Material>(plane_mat));
    shape.SetTransform(commontypes::TranslationMatrix{0, -1, 0});

    auto shape_ptr = std::make_shared<geometry::Plane>(shape);
    w.AddObject(shape_ptr);

    commontypes::Ray r{{0, 0, -3}, {0, -SQRT2OVER2, SQRT2OVER2}};
    const geometry::Intersection i{sqrt(2), shape_ptr};
    auto comps = i.PrepareComputations(r);

    // no remaining recursive calls means we expect the color black
    const auto color = w.ReflectedColor(comps, 0);
    ASSERT_TRUE(color == commontypes::Color::MakeBlack());
}

TEST(WorldTest, TestRefractedColorWithOpaqueSurface) {
    auto w = scene::World::DefaultWorld();
    const auto shape = w.objects().front();
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    const auto xs = std::vector<geometry::Intersection>{geometry::Intersection{4, shape},
                                                        geometry::Intersection{6, shape}};
    auto comps = xs.front().PrepareComputations(r, xs);
    const commontypes::Color c = w.RefractedColor(comps, 5);
    ASSERT_TRUE(c == commontypes::Color::MakeBlack());
}

TEST(WorldTest, TestRefractedColorAtMaximumRecursiveDepth) {
    auto w = scene::World::DefaultWorld();
    auto shape = w.objects().front();
    shape->Material()->SetTransparency(1.0);
    shape->Material()->SetRefractiveIndex(1.5);
    commontypes::Ray r{commontypes::Point{0, 0, -5}, commontypes::Vector{0, 0, 1}};
    const auto xs = std::vector<geometry::Intersection>{geometry::Intersection{4, shape},
                                                        geometry::Intersection{6, shape}};

    auto comps = xs.front().PrepareComputations(r, xs);
    const commontypes::Color c = w.RefractedColor(comps, 0);

    // expects color black when total internal reflection occurs (Ray reflects off surface)
    ASSERT_TRUE(c == commontypes::Color::MakeBlack());
}

/*
 see pg. 156

 total internal reflection occurs when light enters medium at an
 sufficiently acute angle + the new medium has a lower refractive index than the old material

 when this occurs, the ray will reflect off the interface instead of passing through it
*/
TEST(WorldTest, TestRefractedColorUnderTotalInternalReflection) {
    const double sqrt2_over2 = sqrt(2) / 2;

    auto w = scene::World::DefaultWorld();
    auto shape = w.objects().front();
    shape->Material()->SetTransparency(1.0);
    shape->Material()->SetRefractiveIndex(1.5);

    commontypes::Ray r{commontypes::Point{0, 0, sqrt2_over2}, commontypes::Vector{0, 1, 0}};
    const auto xs = std::vector<geometry::Intersection>{
        geometry::Intersection{-sqrt2_over2, shape}, geometry::Intersection{sqrt2_over2, shape}};

    auto comps = xs.at(1).PrepareComputations(r, xs);
    const commontypes::Color c = w.RefractedColor(comps, 5);
    const auto expected_color = commontypes::Color::MakeBlack();

    ASSERT_TRUE(c == expected_color);
}

// tests the remaining cases that do not fall under total internal reflection (handles refraction)
TEST(WorldTest, TestRefractedColorWithRefractedRay) {
    auto w = scene::World::DefaultWorld();
    auto A = w.objects().front();

    auto pattern_ptr = std::make_shared<TestPattern>(TestPattern{});
    const lighting::Material material_a =
        lighting::MaterialBuilder()
            .WithAmbient(1.0)  // fully ambient, so shows regardless of lighting
            .WithPatternPtr(std::move(pattern_ptr))
            .Build();  // WARNING: attempting to build using the call op results in the pattern_ptr
                       // being NULL

    A->SetMaterial(std::make_shared<lighting::Material>(material_a));

    auto B = w.objects().at(1);
    // glassy material
    const lighting::Material material_b =
        lighting::MaterialBuilder().WithTransparency(1.0).WithRefractiveIndex(1.5);
    B->SetMaterial(std::make_shared<lighting::Material>(material_b));

    // ray inside the innermost Sphere, pointing directly upward
    commontypes::Ray r{commontypes::Point{0, 0, 0.1}, commontypes::Vector{0, 1, 0}};
    const std::vector<geometry::Intersection> xs = {
        {-0.9899, A}, {-0.4899, B}, {0.4899, B}, {0.9899, A}};

    auto comps = xs.at(2).PrepareComputations(r, xs);
    const auto c = w.RefractedColor(comps, 5);

    const auto expected = commontypes::Color(0, 0.99888, 0.04725);
    ASSERT_TRUE(c == expected);
}

// see pg. 158-159
TEST(WorldTest, TestShadeHitWithTransparentMaterial) {
    auto w = scene::World::DefaultWorld();

    // glass floor below the two default world's spheres
    auto floor = geometry::Plane();
    floor.SetTransform(commontypes::TranslationMatrix(0, -1, 0));
    const lighting::Material floor_material =
        lighting::MaterialBuilder().WithTransparency(0.5).WithRefractiveIndex(1.5);

    floor.SetMaterial(std::make_shared<lighting::Material>(floor_material));
    w.AddObject(std::move(std::make_shared<geometry::Plane>(floor)));

    // new sphere below the floor
    auto ball = geometry::Sphere();
    const lighting::Material ball_material =
        lighting::MaterialBuilder().WithColor(commontypes::Color{1, 0, 0}).WithAmbient(0.5);

    ball.SetMaterial(std::make_shared<lighting::Material>(ball_material));
    ball.SetTransform(commontypes::TranslationMatrix{0, -3.5, -0.5});
    w.AddObject(std::move(std::make_shared<geometry::Sphere>(ball)));

    auto r = commontypes::Ray{commontypes::Point{0, 0, -3},
                              commontypes::Vector{0, -sqrt(2) / 2, sqrt(2) / 2}};

    const std::vector<geometry::Intersection> xs = {
        {sqrt(2), std::move(std::make_shared<geometry::Plane>(floor))}};

    geometry::Computations comps = xs.front().PrepareComputations(r, xs);
    const auto color = w.ShadeHit(comps, 5);

    // plane is only semi-transparent, so resulting color should combine refracted color of
    // the ball and the color of the Plane
    ASSERT_TRUE(color == commontypes::Color(0.93642, 0.68642, 0.68642));
}