#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include "camera.h"
#include "checkerpattern.h"
#include "color.h"
#include "material.h"
#include "plane.h"
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

// example fromm chapter 9 using the previous chapters' Spheres with the addition of a
// Plane for the "floor" in the image
void Chapter10PatternPlaneRender() {
    scene::World world{};
    scene::Camera camera{CAMERA_HEIGHT, CAMERA_WIDTH, M_PI / 3};

    const commontypes::Point from{0, 1.5, -5};
    const commontypes::Point to{0, 1, 0};
    const commontypes::Vector up{0, 1, 0};

    const commontypes::ViewTransform camera_transform{from, to, up};
    camera.SetTransform(camera_transform);

    auto world_light =
        lighting::PointLight{commontypes::Point{-10, 10, -10}, commontypes::Color{1, 1, 1}};
    world.SetLight(std::make_shared<lighting::PointLight>(world_light));

    geometry::Plane plane;
    lighting::Material plane_mat = lighting::MaterialBuilder().WithReflective(0.3);

    auto mat_ptr = std::make_shared<lighting::Material>(plane_mat);
    plane.SetMaterial(mat_ptr);

    pattern::CheckerPattern checker_pattern(commontypes::Color::MakeWhite(),
                                            commontypes::Color::MakeBlack());

    const auto pattern_ptr = std::make_shared<pattern::CheckerPattern>(checker_pattern);
    mat_ptr->SetPattern(pattern_ptr);

    world.AddObject(std::make_shared<geometry::Plane>(plane));

    auto sphere_vec = GetSpheresForCh7Render();
    world.AddObjects(std::move(sphere_vec));

    WriteCanvasToPPM(camera, world);
}

}  // namespace

int main() {
    std::function<void()> render_fn = &Chapter10PatternPlaneRender;
    utility::OutputMeasuredDuration(render_fn);
}
