#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <string>
#include "camera.h"
#include "canvas.h"
#include "checkerpattern.h"
#include "color.h"
#include "cylinder.h"
#include "identitymatrix.h"
#include "lighting.h"
#include "material.h"
#include "plane.h"
#include "pointlight.h"
#include "ray.h"
#include "rotationmatrix.h"
#include "scalingmatrix.h"
#include "sphere.h"
#include "translationmatrix.h"
#include "viewtransform.h"
#include "world.h"

#include <gflags/gflags.h>

// TODO: ensure aspect ratio is preserved-(ish), and values are otherwise valid (i.e width >
// height)
static constexpr int DEFAULT_CAMERA_HEIGHT = 900;
static constexpr int DEFAULT_CAMERA_WIDTH = 750;

size_t camera_height = DEFAULT_CAMERA_HEIGHT;
size_t camera_width = DEFAULT_CAMERA_WIDTH;

const std::set<std::string> VALID_TARGETS = {"chapter7", "chapter6", "chapter10",
                                             "patternroomsphere", "patternroomcylinder"};

const std::string USAGE_MESSAGE = R"(
-width (Camera Width for the render)
-height (Camera Height fmr the render)
-renderTarget (Target scene to render. One of: (chapter10, chapter6, chapter7, patternroomcylinder, patternroomsphere))
)";

const std::string RENDER_HELP_STR =
    "Name of the render example (" +
    std::accumulate(std::next(VALID_TARGETS.begin()),
                    VALID_TARGETS.end(),
                    *VALID_TARGETS.begin(),
                    [](const std::string& acc, const std::string& s) { return acc + ", " + s; }) +
    ")";

DEFINE_string(renderTarget, "", RENDER_HELP_STR.c_str());
DEFINE_int32(width, DEFAULT_CAMERA_WIDTH, "Camera Width for the render");
DEFINE_int32(height, DEFAULT_CAMERA_HEIGHT, "Camera Height for the render");

namespace {
void WriteCanvasToPPM(const scene::Camera& camera, scene::World& world) {
    const auto canvas = camera.Render(world);
    std::string image_outdir_name = "images";

    utility::CreateImageOutdir(image_outdir_name);
    const std::string outfile_name =
        std::move(image_outdir_name) + "/" + utility::CurrentDateStr() + "_image.ppm";
    std::ofstream out{outfile_name};

    out << canvas.WritePPM();
    out.close();
}

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
    scene::Camera camera{camera_height, camera_width, M_PI / 3};
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

void Chapter6RenderRenderExample(
    const std::optional<commontypes::Matrix>& transform_matrix = std::nullopt) {
    commontypes::Point ray_origin{0, 0, -5};
    double wall_z = 10;
    double wall_size = 7.0;
    double half = wall_size / 2;
    uint16_t canvas_pixels = 1000;
    double pixel_size = wall_size / canvas_pixels;
    canvas::Canvas canvas{canvas_pixels, canvas_pixels};

    geometry::Sphere shape;
    if (transform_matrix.has_value()) {
        shape.SetTransform(transform_matrix.value());
    }

    std::shared_ptr<lighting::Material> m_ptr = std::make_shared<lighting::Material>();
    shape.SetMaterial(m_ptr);
    m_ptr->SetColor(commontypes::Color{1, 0.2, 1});

    commontypes::Point light_position{-10, 10, -10};
    commontypes::Color light_color{1, 1, 1};
    lighting::PointLight light{light_position, light_color};

    for (int y = 0; y < canvas.height() - 1; ++y) {
        std::clog << '\r' << "Scanlines remaining: " << (canvas.height() - y) << " " << std::flush;

        const double world_y = half - pixel_size * y;

        for (int x = 0; x < canvas.width() - 1; ++x) {
            double world_x = -half + pixel_size * x;
            commontypes::Point position{world_x, world_y, wall_z};
            commontypes::Ray r{ray_origin,
                               commontypes::Vector{(position - ray_origin).Normalize()}};
            auto xs = shape.Intersect(r);

            if (!xs.empty()) {
                geometry::Intersection hit = xs.at(0);
                commontypes::Point point = r.Position(hit.t_);
                commontypes::Vector normal = hit.object_->NormalAt(point);
                commontypes::Vector eye = commontypes::Vector{-r.direction()};

                commontypes::Color color = lighting::Lighting(
                    hit.object_->Material(), commontypes::IdentityMatrix{},
                    std::make_shared<lighting::PointLight>(light), point, eye, normal);
                canvas.WritePixel(x, y, color);
            }
        }
    }

    const std::string image_outdir_name = "images";
    utility::CreateImageOutdir(image_outdir_name);
    std::ofstream out{image_outdir_name + "/" + utility::CurrentDateStr() + "image.ppm"};

    out << canvas.WritePPM();
    out.close();
}

// example fromm chapter 9 using the previous chapters' Spheres with the addition of a
// Plane for the "floor" in the image
void Chapter10PatternPlaneRender() {
    scene::World world{};
    scene::Camera camera{camera_height, camera_width, M_PI / 3};

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

    scene::Camera camera{camera_height, camera_width, M_PI / 4.0};

    commontypes::Point from = commontypes::Point(10, 1, 0);
    commontypes::Point to = commontypes::Point(0.0, 0.0, 0.0);
    commontypes::Vector up = commontypes::Vector(0.0, 1.0, 0.0);

    camera.SetTransform(commontypes::ViewTransform{from, to, up});

    WriteCanvasToPPM(camera, world);
}

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

    scene::Camera camera{camera_height, camera_width, M_PI / 4.0};

    const commontypes::Point from = commontypes::Point(10, 1, 0);
    const commontypes::Point to = commontypes::Point(0.0, 0.0, 0.0);
    const commontypes::Vector up = commontypes::Vector(0.0, 1.0, 0.0);

    camera.SetTransform(commontypes::ViewTransform{from, to, up});

    WriteCanvasToPPM(camera, world);
}

bool AreFlagValuesValid() {
    bool flags_valid{true};

    if (FLAGS_width <= 0) {
        std::cerr << "Error: 'width' must be a positive integer.\n";
        flags_valid = false;
    }
    if (FLAGS_height <= 0) {
        std::cerr << "Error: 'height' must be a positive integer.\n";
        flags_valid = false;
    }
    if (FLAGS_renderTarget.empty()) {
        std::cerr << "Error: 'renderTarget' must not be empty.\n";
        // we dont need the error message that follows this one shown as well, if this is the case
        return false;
    }
    if (VALID_TARGETS.find(FLAGS_renderTarget) == VALID_TARGETS.end()) {
        std::cerr << "Error: "
                  << "'" << FLAGS_renderTarget << "' is not a valid target: " << RENDER_HELP_STR
                  << "\n";
        flags_valid = false;
    }
    return flags_valid;
}

std::function<void()> GetRenderFunction(const std::string& render_target) {
    std::map<std::string, std::function<void()>> RENDER_TARGETS = {
        {"chapter7", RenderChapter7Scene},
        {"chapter6", []() { Chapter6RenderRenderExample(); }},
        {"chapter10", Chapter10PatternPlaneRender},
        {"patternroomsphere", PatternRoomRefractiveSphere},
        {"patternroomcylinder", PatternRoomRefractiveCylinder}};

    return RENDER_TARGETS[render_target];
}
}  // namespace

int main(int argc, char* argv[]) {
    gflags::SetUsageMessage(USAGE_MESSAGE);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    const bool flags_valid = AreFlagValuesValid();
    if (!flags_valid) {
        std::cerr << gflags::ProgramUsage() << std::endl;
        exit(EXIT_FAILURE);
    };

    camera_height = FLAGS_height;
    camera_width = FLAGS_width;

    std::function<void()> render_fn = GetRenderFunction(FLAGS_renderTarget);
    utility::OutputMeasuredDuration(render_fn);
}
