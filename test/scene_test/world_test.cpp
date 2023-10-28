#include "world.h"
#include <gtest/gtest.h>
#include "scalingmatrix.h"
#include "translationmatrix.h"

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

    auto s1_ptr = std::make_shared<geometry::Sphere>(s1);
    auto s2_ptr = std::make_shared<geometry::Sphere>(s2);
    std::vector<std::shared_ptr<geometry::Sphere>> v1{s1_ptr, s2_ptr};

    world.AddObjects(std::move(v1));
    ASSERT_EQ(world.objects().size(), 2);
    ASSERT_TRUE(world.WorldContains(s1_ptr));
    ASSERT_TRUE(world.WorldContains(s2_ptr));

    auto s3_ptr = std::make_shared<geometry::Sphere>(s3);
    auto s4_ptr = std::make_shared<geometry::Sphere>(s4);
    std::vector<std::shared_ptr<geometry::Sphere>> v2{s3_ptr, s4_ptr};

    world.AddObjects(std::move(v2));
    ASSERT_EQ(world.objects().size(), 4);
    ASSERT_TRUE(world.WorldContains(s3_ptr));
    ASSERT_TRUE(world.WorldContains(s4_ptr));
}

TEST(WorldTest, TestDefaultWorld) {
    scene::World default_world = scene::World::DefaultWorld();

    lighting::PointLight light{commontypes::Point{-10, 10, -10}, commontypes::Color{1, 1, 1}};

    geometry::Sphere s1{};
    lighting::Material material{};
    material.SetColor(commontypes::Color{0.8, 1.0, 0.6});
    material.SetDiffuse(0.7);
    material.SetSpecular(0.2);
    s1.SetMaterial(std::make_shared<lighting::Material>(material));

    geometry::Sphere s2{};
    s2.SetTransform(commontypes::ScalingMatrix{0.5, 0.5, 0.5});

    auto world_light = *default_world.light();
    ASSERT_TRUE(world_light == light);
    ASSERT_TRUE(default_world.WorldContains(std::make_shared<geometry::Sphere>(s1)));
    ASSERT_TRUE(default_world.WorldContains(std::make_shared<geometry::Sphere>(s2)));
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
    commontypes::Color c = default_world.ShadeHit(comps);
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
    commontypes::Color c = default_world.ShadeHit(comps);
    ASSERT_TRUE(c == commontypes::Color(0.90498, 0.90498, 0.90498));
}

TEST(WorldTest, TestColorWhenRayMisses) {
    scene::World default_world = scene::World::DefaultWorld();
    commontypes::Ray r{{0, 0, -5}, {0, 1, 0}};
    commontypes::Color c = default_world.ColorAt(r);
    ASSERT_TRUE(c == commontypes::Color(0, 0, 0));
}

TEST(WorldTest, TestColorWhenRayHits) {
    scene::World default_world = scene::World::DefaultWorld();
    commontypes::Ray r{{0, 0, -5}, {0, 0, 1}};
    commontypes::Color c = default_world.ColorAt(r);
    ASSERT_TRUE(c == commontypes::Color(0.38066, 0.47583, 0.2855));
}

TEST(WorldTest, TestColorWhenIntersectionBehindRay) {
    scene::World default_world = scene::World::DefaultWorld();

    auto outer = default_world.objects().at(0);
    outer->material()->SetAmbient(1);

    auto inner = default_world.objects().at(1);
    inner->material()->SetAmbient(1);

    commontypes::Ray r{{0, 0, 0.75}, {0, 0, -1}};
    commontypes::Color c = default_world.ColorAt(r);
    ASSERT_TRUE(c == inner->material()->Color());
}

TEST(WorldTest, TestNoShadowWhenNothingIsCollinearWithPointAndLight) {
    scene::World w = scene::World::DefaultWorld();
    commontypes::Point p{0, 10, 0};
    const auto is_shadowed = w.IsShadowed(p);
    EXPECT_FALSE(is_shadowed);
}

TEST(WorldTest, TestTheShadowWhenObjectIsBetweenPointAndLight) {
    scene::World w = scene::World::DefaultWorld();
    commontypes::Point p{10, -10, 10};
    const auto is_shadowed = w.IsShadowed(p);
    EXPECT_TRUE(is_shadowed);
}

TEST(WorldTest, TestThereIsNoShadowWhenObjectIsBehindTheLight) {
    scene::World w = scene::World::DefaultWorld();
    commontypes::Point p{-20, 20, -20};
    const auto is_shadowed = w.IsShadowed(p);
    EXPECT_FALSE(is_shadowed);
}

TEST(WorldTest, TestThereIsNoShadowWhenObjectIsBehindThePoint) {
    scene::World w = scene::World::DefaultWorld();
    commontypes::Point p{-2, 2, -2};
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

    w.AddObjects(std::vector<std::shared_ptr<geometry::Sphere>>{s1, s2});
    commontypes::Ray r{{0, 0, 5}, {0, 0, 1}};
    geometry::Intersection i{4, s2};
    auto comps = i.PrepareComputations(r);
    commontypes::Color c = w.ShadeHit(comps);

    ASSERT_TRUE(c == commontypes::Color(0.1, 0.1, 0.1));
}