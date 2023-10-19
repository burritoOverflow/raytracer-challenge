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
