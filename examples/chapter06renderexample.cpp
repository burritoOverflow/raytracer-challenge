#include <fstream>
#include <iostream>
#include <memory>
#include "canvas.h"
#include "checkerpattern.h"
#include "color.h"
#include "cylinder.h"
#include "identitymatrix.h"
#include "lighting.h"
#include "material.h"
#include "pointlight.h"
#include "ray.h"
#include "sphere.h"

namespace {

void Chapter6RenderExample() {
    commontypes::Point ray_origin{0, 0, -5};
    double wall_z = 10;
    double wall_size = 7.0;
    double half = wall_size / 2;
    uint16_t canvas_pixels = 1000;
    double pixel_size = wall_size / canvas_pixels;
    canvas::Canvas canvas{canvas_pixels, canvas_pixels};

    geometry::Sphere shape;
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

}  // namespace

int main() {
    std::function<void()> render_fn = &Chapter6RenderExample;
    utility::OutputMeasuredDuration(render_fn);
}
