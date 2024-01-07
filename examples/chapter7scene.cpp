#include <functional>
#include <memory>
#include "camera.h"
#include "checkerpattern.h"
#include "color.h"
#include "material.h"
#include "pointlight.h"
#include "ray.h"
#include "rotationmatrix.h"
#include "scalingmatrix.h"
#include "sphere.h"
#include "translationmatrix.h"
#include "util.h"
#include "viewtransform.h"
#include "world.h"

namespace {

std::vector<std::shared_ptr<geometry::Shape>> GetSpheresForCh7Render() {
    auto left_sphere = geometry::Sphere{};
    const auto left_transform = commontypes::TranslationMatrix{-1.5, 0.33, -0.75} *
                                commontypes::ScalingMatrix{0.33, 0.33, 0.33};
    left_sphere.SetTransform(left_transform);

    lighting::Material left_sphere_mat = lighting::MaterialBuilder()
                                             .WithColor(commontypes::Color{0, 0, 1})
                                             .WithDiffuse(0.7)
                                             .WithSpecular(0.3)
                                             .WithReflective(0.9);
    left_sphere.SetMaterial(std::make_shared<lighting::Material>(std::move(left_sphere_mat)));

    auto middle_sphere = geometry::Sphere{};
    middle_sphere.SetTransform(commontypes::TranslationMatrix{-0.5, 1, 0.5});

    lighting::Material middle_sphere_mat = lighting::MaterialBuilder()
                                               .WithColor(commontypes::Color{0.55, 0, 0})
                                               .WithTransparency(0.9)
                                               .WithRefractiveIndex(1.6)
                                               .WithReflective(0.8)
                                               .WithDiffuse(0.22)
                                               .WithSpecular(0.33);
    middle_sphere.SetMaterial(std::make_shared<lighting::Material>(std::move(middle_sphere_mat)));

    auto right_sphere = geometry::Sphere{};
    const auto right_sphere_transform =
        commontypes::TranslationMatrix{1.5, 0.5, -0.5} * commontypes::ScalingMatrix{0.5, 0.5, 0.5};
    right_sphere.SetTransform(right_sphere_transform);

    lighting::Material right_sphere_mat = lighting::MaterialBuilder()
                                              .WithColor(commontypes::Color{0, 0.79, 0})
                                              .WithDiffuse(0.7)
                                              .WithSpecular(0.55)
                                              .WithReflective(0.8);
    right_sphere.SetMaterial(std::make_shared<lighting::Material>(std::move(right_sphere_mat)));

    return std::vector<std::shared_ptr<geometry::Shape>>{
        std::make_shared<geometry::Sphere>(left_sphere),
        std::make_shared<geometry::Sphere>(middle_sphere),
        std::make_shared<geometry::Sphere>(right_sphere)};
}

void RenderChapter7Scene() {
    scene::World world{};
    scene::Camera camera{CAMERA_HEIGHT, CAMERA_WIDTH, M_PI / 3};
    camera.SetTransform(commontypes::ViewTransform{commontypes::Point{0, 1.5, -5},
                                                   commontypes::Point{0, 1, 0},
                                                   commontypes::Vector{0, 1, 0}});

    auto floor_material = lighting::Material{};
    floor_material.SetSpecular(0);
    floor_material.SetColor(commontypes::Color{1, 0.9, 0.9});
    auto floor_mat_ptr = std::make_shared<lighting::Material>(floor_material);

    auto floor = geometry::Sphere{};
    floor.SetTransform(commontypes::ScalingMatrix{10, 0.01, 10});
    floor.SetMaterial(floor_mat_ptr);

    auto left_wall = geometry::Sphere{};
    auto transform = commontypes::TranslationMatrix{0, 0, 5} *
                     commontypes::RotationMatrixY{-M_PI_4} * commontypes::RotationMatrixX{M_PI_2} *
                     commontypes::ScalingMatrix{10, 0.01, 10};
    left_wall.SetTransform(transform);
    left_wall.SetMaterial(floor_mat_ptr);

    auto right_wall = geometry::Sphere{};
    auto r_transform =
        commontypes::TranslationMatrix{0, 0, 5} * commontypes::RotationMatrixY{M_PI_4} *
        commontypes::RotationMatrixX{M_PI_2} * commontypes::ScalingMatrix{10, 0.01, 10};
    right_wall.SetTransform(r_transform);
    right_wall.SetMaterial(floor_mat_ptr);

    auto world_light =
        lighting::PointLight{commontypes::Point{-10, 10, -10}, commontypes::Color{1, 1, 1}};
    world.SetLight(std::make_shared<lighting::PointLight>(world_light));

    world.AddObjects(std::vector<std::shared_ptr<geometry::Shape>>({
        std::make_shared<geometry::Sphere>(floor),
        std::make_shared<geometry::Sphere>(left_wall),
        std::make_shared<geometry::Sphere>(right_wall),
    }));

    auto sphere_vec = GetSpheresForCh7Render();
    world.AddObjects(std::move(sphere_vec));

    WriteCanvasToPPM(camera, world);
}
}  // namespace

int main() {
    std::function<void()> render_fn = &RenderChapter7Scene;
    utility::OutputMeasuredDuration(render_fn);
}
