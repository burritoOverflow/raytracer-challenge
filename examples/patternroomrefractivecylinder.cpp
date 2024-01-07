#include <functional>
#include <memory>
#include "camera.h"
#include "checkerpattern.h"
#include "color.h"
#include "cylinder.h"
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

void PatternRoomRefractiveCylinder() {
    scene::World world{};
    auto light = lighting::PointLight{commontypes::Point{-1, 20, 0}, commontypes::Color{1, 0, 0}};
    world.SetLight(std::make_shared<lighting::PointLight>(light));

    auto plane = geometry::Plane();
    pattern::CheckerPattern checker_pattern(commontypes::Color::MakeWhite(),
                                            commontypes::Color::MakeBlack());

    plane.Material()->SetPattern(
        std::make_shared<pattern::CheckerPattern>(std::move(checker_pattern)));
    plane.Material()->SetReflective(0);
    plane.Material()->SetShininess(30);

    world.AddObject(std::make_shared<geometry::Plane>(plane));

    auto back_wall = geometry::Plane{};
    back_wall.SetMaterial(plane.Material());
    back_wall.SetTransform(commontypes::TranslationMatrix{-3, 0, 0} *
                           commontypes::RotationMatrixZ{M_PI_2});

    world.AddObject(std::make_shared<geometry::Plane>(std::move(back_wall)));

    auto cylinder = geometry::Cylinder{0, 3.5, true};
    lighting::Material cylinder_material = lighting::MaterialBuilder()
                                               .WithTransparency(0.8)
                                               .WithRefractiveIndex(1.5)
                                               .WithColor(commontypes::Color{})
                                               .WithReflective(0.2);

    cylinder.SetMaterial(std::make_shared<lighting::Material>(std::move(cylinder_material)));
    cylinder.SetTransform(commontypes::TranslationMatrix{3.5, 0.2, -1.2} *
                          commontypes::ScalingMatrix{1, 0.67, 1});

    world.AddObject(std::make_shared<geometry::Cylinder>(cylinder));

    auto red_sphere = geometry::Sphere{};
    red_sphere.SetTransform(commontypes::TranslationMatrix{-1.5, 1, 1});
    lighting::Material red_sphere_mat =
        lighting::MaterialBuilder().WithColor(commontypes::Color{0.68, .44, 0});

    red_sphere.SetMaterial(std::make_shared<lighting::Material>(std::move(red_sphere_mat)));

    world.AddObject(std::make_shared<geometry::Sphere>(std::move(red_sphere)));

    scene::Camera camera{CAMERA_HEIGHT, CAMERA_WIDTH, M_PI / 4.0};

    const commontypes::Point from = commontypes::Point(10, 1, 0);
    const commontypes::Point to = commontypes::Point(0.0, 0.0, 0.0);
    const commontypes::Vector up = commontypes::Vector(0.0, 1.0, 0.0);

    camera.SetTransform(commontypes::ViewTransform{from, to, up});

    WriteCanvasToPPM(camera, world);
}
}  // namespace

int main() {
    std::function<void()> render_fn = &PatternRoomRefractiveCylinder;
    utility::OutputMeasuredDuration(render_fn);
}
