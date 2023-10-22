#include <filesystem>
#include <fstream>
#include <iostream>
#include "canvas.h"
#include "color.h"
#include "lighting.h"
#include "pointlight.h"
#include "ray.h"
#include "scalingmatrix.h"
#include "sphere.h"

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

int main() {
    Chapter6RenderRenderExample();
}
