#include "world.h"

#include <utility>
#include "scalingmatrix.h"

// see description of the "Default World" on pg. 92
scene::World scene::World::DefaultWorld() {
    scene::World world{};
    lighting::PointLight light{commontypes::Point{-10, 10, -10}, commontypes::Color{1, 1, 1}};
    world.SetLight(std::make_shared<lighting::PointLight>(light));

    lighting::Material material{};
    material.SetColor(commontypes::Color{0.8, 1.0, 0.6});
    material.SetDiffuse(0.7);
    material.SetSpecular(0.2);

    geometry::Sphere s1{};
    s1.SetMaterial(std::make_shared<lighting::Material>(material));

    geometry::Sphere s2{};
    s2.SetTransform(commontypes::ScalingMatrix{0.5, 0.5, 0.5});

    world.AddObject(std::make_shared<geometry::Sphere>(s1));
    world.AddObject(std::make_shared<geometry::Sphere>(s2));

    return world;
}

void scene::World::AddObject(const std::shared_ptr<geometry::Sphere>& object) {
    objects_.insert(objects_.end(), object);
}

void scene::World::SetLight(std::shared_ptr<lighting::PointLight> light) {
    light_ = std::move(light);
}
