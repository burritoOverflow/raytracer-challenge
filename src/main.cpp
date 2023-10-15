#include <filesystem>
#include <fstream>
#include <iostream>
#include "canvas.h"
#include "color.h"
#include "ray.h"
#include "scalingmatrix.h"
#include "shearingmatrix.h"
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

void Chapter5RenderFirstRenderExample(const std::optional<commontypes::Matrix>& transform_matrix) {
    commontypes::Point ray_origin{0, 0, -5};
    double wall_z = 10;
    double wall_size = 7.0;
    double half = wall_size / 2;
    uint16_t canvas_pixels = 1000;
    double pixel_size = wall_size / canvas_pixels;
    Canvas canvas{canvas_pixels, canvas_pixels};

    commontypes::Color red{1, 0, 0};
    geometry::Sphere shape;
    if (transform_matrix.has_value()) {
        shape.SetTransform(transform_matrix.value());
    }

    // each row of pixels in the Canvas
    for (int y = 0; y < canvas.height() - 1; ++y) {
        std::clog << '\r' << "Scanlines remaining: " << (canvas.height() - y) << " " << std::flush;

        // world y coordinate top == +half, bottom = -half
        double world_y = half - pixel_size * y;

        // each pixel in the row
        for (int x = 0; x < canvas.width() - 1; ++x) {
            double world_x = -half + pixel_size * x;
            commontypes::Point position{world_x, world_y, wall_z};
            commontypes::Ray r{ray_origin, (position - ray_origin).Normalize()};
            auto xs = shape.Intersect(r);
            if (!xs.empty()) {
                canvas.WritePixel(x, y, red);
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
    // example from book
    commontypes::ShearingMatrix shearing_matrix{1, 0, 0, 0, 0, 0};
    commontypes::ScalingMatrix scaling_matrix{0.5, 1, 1};
    commontypes::Matrix transform = shearing_matrix * scaling_matrix;
    Chapter5RenderFirstRenderExample(transform);
}