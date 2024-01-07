#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include "camera.h"
#include "world.h"

constexpr int CAMERA_HEIGHT = 900;
constexpr int CAMERA_WIDTH = 750;

inline void WriteCanvasToPPM(const scene::Camera& camera, scene::World& world) {
    const auto canvas = camera.Render(world);
    std::string image_outdir_name = "images";

    utility::CreateImageOutdir(image_outdir_name);
    const std::string outfile_name =
        std::move(image_outdir_name) + "/" + utility::CurrentDateStr() + "_image.ppm";
    std::ofstream out{outfile_name};

    out << canvas.WritePPM();
    out.close();
}
#endif  // UTIL_H
