#include <functional>
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

// a "room" with a checkered pattern, a transparent Sphere, and a red Sphere offset and positioned
// behind the transparent Sphere
void PatternRoomRefractiveSphere() {
    scene::World world{};
    auto light = lighting::PointLight{commontypes::Point{-1, 20, 0}, commontypes::Color{1, 1, 1}};
    world.SetLight(std::make_shared<lighting::PointLight>(light));

    auto plane = geometry::Plane();
    pattern::CheckerPattern checker_pattern(commontypes::Color::MakeWhite(),
                                            commontypes::Color::MakeBlack());

    plane.Material()->SetPattern(std::make_shared<pattern::CheckerPattern>(checker_pattern));
    plane.Material()->SetReflective(0);
    plane.Material()->SetShininess(30);

    world.AddObject(std::make_shared<geometry::Plane>(plane));

    auto back_wall = geometry::Plane{};
    back_wall.SetMaterial(plane.Material());
    back_wall.SetTransform(commontypes::TranslationMatrix{-3, 0, 0} *
                           commontypes::RotationMatrixZ{M_PI_2});

    world.AddObject(std::make_shared<geometry::Plane>(std::move(back_wall)));

    auto sphere = geometry::Sphere{};
    lighting::Material sphere_mat = lighting::MaterialBuilder()
                                        .WithTransparency(0.8)
                                        .WithRefractiveIndex(1.5)
                                        .WithColor(commontypes::Color{0.3, 0.3, 0.3})
                                        .WithReflective(0.2);

    sphere.SetMaterial(std::make_shared<lighting::Material>(std::move(sphere_mat)));
    sphere.SetTransform(commontypes::TranslationMatrix{3, 1, 0} *
                        commontypes::ScalingMatrix{1, 1, 1});

    world.AddObject(std::make_shared<geometry::Sphere>(sphere));

    auto red_sphere = geometry::Sphere{};
    red_sphere.SetTransform(commontypes::TranslationMatrix{-1.5, 1, 1});
    lighting::Material red_sphere_mat =
        lighting::MaterialBuilder().WithColor(commontypes::Color{1, 0, 0});

    red_sphere.SetMaterial(std::make_shared<lighting::Material>(std::move(red_sphere_mat)));

    world.AddObject(std::make_shared<geometry::Sphere>(std::move(red_sphere)));

    scene::Camera camera{CAMERA_HEIGHT, CAMERA_WIDTH, M_PI / 4.0};

    commontypes::Point from = commontypes::Point(10, 1, 0);
    commontypes::Point to = commontypes::Point(0.0, 0.0, 0.0);
    commontypes::Vector up = commontypes::Vector(0.0, 1.0, 0.0);

    camera.SetTransform(commontypes::ViewTransform{from, to, up});

    WriteCanvasToPPM(camera, world);
}

}  // namespace

int main() {
    std::function<void()> render_fn = &PatternRoomRefractiveSphere;
    utility::OutputMeasuredDuration(render_fn);
}
