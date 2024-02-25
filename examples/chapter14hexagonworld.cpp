#include "hexagon_world_example.h"

#include <functional>
#include <memory>
#include "camera.h"
#include "checkerpattern.h"
#include "color.h"
#include "pointlight.h"
#include "ray.h"
#include "util.h"
#include "viewtransform.h"
#include "world.h"

void RenderHexagonWorld() {
    scene::World world{};
    auto light = std::make_shared<lighting::PointLight>(commontypes::Point{-10, 10, -10},
                                                        commontypes::Color{1, 1, 1});
    world.SetLight(light);
    auto hexagon = MakeHexagon();
    world.AddObject(std::move(hexagon));

    scene::Camera camera{CAMERA_HEIGHT, CAMERA_WIDTH, M_PI / 3};

    commontypes::Point from = commontypes::Point(0, 1.5, -5);
    commontypes::Point to = commontypes::Point(0, 1, 0);
    commontypes::Vector up = commontypes::Vector(0, 1, 0);
    camera.SetTransform(commontypes::ViewTransform{from, to, up});

    WriteCanvasToPPM(camera, world);
}

int main() {
    std::function<void()> render_fn = &RenderHexagonWorld;
    utility::OutputMeasuredDuration(render_fn);
}