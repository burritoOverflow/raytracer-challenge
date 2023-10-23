#include <filesystem>
#include <fstream>
#include <iostream>
#include "camera.h"
#include "canvas.h"
#include "color.h"
#include "lighting.h"
#include "pointlight.h"
#include "ray.h"
#include "rotationmatrix.h"
#include "scalingmatrix.h"
#include "sphere.h"
#include "translationmatrix.h"
#include "viewtransform.h"
#include "world.h"

namespace {
void CreateImageOutdir(const std::string_view dirname) {
    if (!std::filesystem::exists(dirname)) {
        std::filesystem::create_directory(dirname);
    }
}

std::string CurrentDateStr() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    const std::string date_fmt_str = "%m-%d-%Y_%H-%M-%S";
    oss << std::put_time(&tm, date_fmt_str.c_str());
    return oss.str();
}

void RenderChapter7Scene() {
    scene::World world{};
    scene::Camera camera{900, 750, M_PI / 3};
    camera.SetTransform(commontypes::ViewTransform{{0, 1.5, -5}, {0, 1, 0}, {0, 1, 0}});

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

    auto left_sphere = geometry::Sphere{};
    const auto left_transform = commontypes::TranslationMatrix{-1.5, 0.33, -0.75} *
                                commontypes::ScalingMatrix{0.33, 0.33, 0.33};
    left_sphere.SetTransform(left_transform);
    auto left_sphere_mat = lighting::Material{};
    left_sphere_mat.SetColor({1, 0.8, 0.1});
    left_sphere_mat.SetDiffuse(0.7);
    left_sphere_mat.SetSpecular(0.3);
    left_sphere.SetMaterial(std::make_shared<lighting::Material>(left_sphere_mat));

    auto middle_sphere = geometry::Sphere{};
    middle_sphere.SetTransform(commontypes::TranslationMatrix{-0.5, 1, 0.5});
    auto middle_sphere_mat = lighting::Material{};
    middle_sphere_mat.SetColor(commontypes::Color{0.1, 1, 0.5});
    middle_sphere_mat.SetDiffuse(0.7);
    middle_sphere_mat.SetSpecular(0.3);
    middle_sphere.SetMaterial(std::make_shared<lighting::Material>(middle_sphere_mat));

    auto right_sphere = geometry::Sphere{};
    const auto right_sphere_transform =
        commontypes::TranslationMatrix{1.5, 0.5, -0.5} * commontypes::ScalingMatrix{0.5, 0.5, 0.5};
    auto right_sphere_mat = lighting::Material{};
    right_sphere.SetTransform(right_sphere_transform);
    right_sphere_mat.SetColor({0.5, 1, 0.1});
    right_sphere_mat.SetDiffuse(0.7);
    right_sphere_mat.SetSpecular(0.3);
    right_sphere.SetMaterial(std::make_shared<lighting::Material>(right_sphere_mat));

    auto world_light = lighting::PointLight{{-10, 10, -10}, {1, 1, 1}};
    world.SetLight(std::make_shared<lighting::PointLight>(world_light));

    world.AddObjects(std::vector<std::shared_ptr<geometry::Sphere>>(
        {std::make_shared<geometry::Sphere>(floor), std::make_shared<geometry::Sphere>(left_wall),
         std::make_shared<geometry::Sphere>(right_wall),
         std::make_shared<geometry::Sphere>(middle_sphere),
         std::make_shared<geometry::Sphere>(left_sphere),
         std::make_shared<geometry::Sphere>(right_sphere)}));

    const auto canvas = camera.Render(world);
    std::string image_outdir_name = "images";
    CreateImageOutdir(image_outdir_name);
    std::ofstream out{std::move(image_outdir_name) + "/" + CurrentDateStr() + "image.ppm"};

    out << canvas.WritePPM();
    out.close();
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

        double world_y = half - pixel_size * y;

        for (int x = 0; x < canvas.width() - 1; ++x) {
            double world_x = -half + pixel_size * x;
            commontypes::Point position{world_x, world_y, wall_z};
            commontypes::Ray r{ray_origin, (position - ray_origin).Normalize()};
            auto xs = shape.Intersect(r);
            if (!xs.empty()) {
                geometry::Intersection hit = xs.at(0);
                commontypes::Point point = r.Position(hit.t_);
                commontypes::Vector normal = hit.object_->NormalAt(point);
                commontypes::Vector eye = -r.direction();

                commontypes::Color color = lighting::Lighting(
                    hit.object_->material(), std::make_shared<lighting::PointLight>(light), point,
                    eye, normal);
                canvas.WritePixel(x, y, color);
            }
        }
    }

    const std::string image_outdir_name = "images";
    CreateImageOutdir(image_outdir_name);
    std::ofstream out{image_outdir_name + "/" + CurrentDateStr() + "image.ppm"};

    out << canvas.WritePPM();
    out.close();
}
}  // namespace

int main() {
    RenderChapter7Scene();
}
