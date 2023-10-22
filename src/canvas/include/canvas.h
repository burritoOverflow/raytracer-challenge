#ifndef CANVAS_H
#define CANVAS_H

#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include "color.h"

using raytracercanvas = std::vector<std::vector<commontypes::Color>>;

namespace canvas {
class Canvas final {
   public:
    Canvas(const size_t width, const size_t height) : width_(width), height_(height) {
        std::vector<commontypes::Color> height_vector{height_};
        canvas_.resize(width_, height_vector);

        for (size_t x = 0; x < width_; x++) {
            for (size_t y = 0; y < height_; y++) {
                canvas_.at(x).at(y) = commontypes::Color{};
            }
        }
    }

    inline size_t width() const { return width_; }
    inline size_t height() const { return height_; }
    inline raytracercanvas GetCurrentCanvas() { return canvas_; }

    inline commontypes::Color GetPixel(const size_t x, const size_t y) const {
        assert(x >= 0 && x < width_);
        assert(y >= 0 && y < height_);
        return canvas_.at(x).at(y);
    }

    inline void WritePixel(const size_t x, const size_t y, commontypes::Color& color) {
        assert(x >= 0 && x < width_);
        assert(y >= 0 && y < height_);
        canvas_.at(x).at(y) = color;
    }

    // write the current canvas contents to a PPM file
    std::string WritePPM() const;

   private:
    static double Clamp(double d, double min = 0.0, double max = 0.999);

    size_t width_;
    size_t height_;
    raytracercanvas canvas_;
};
}  // namespace canvas

#endif  // CANVAS_H
