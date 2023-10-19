#include "world.h"
#include <gtest/gtest.h>
#include "scalingmatrix.h"

TEST(WorldTest, TestCreatingWorld) {
    scene::World world{};
    ASSERT_TRUE(world.light() == nullptr);
    ASSERT_TRUE(world.objects().empty());
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

    ASSERT_FALSE(std::find(default_world.objects().begin(), default_world.objects().end(),
                           std::make_shared<geometry::Sphere>(s1)) ==
                 default_world.objects().end());

    ASSERT_FALSE(std::find(default_world.objects().begin(), default_world.objects().end(),
                           std::make_shared<geometry::Sphere>(s2)) ==
                 default_world.objects().end());
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