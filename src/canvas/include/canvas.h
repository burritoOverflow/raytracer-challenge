#ifndef CANVAS_H
#define CANVAS_H

#include <cassert>
#include <vector>
#include "color.h"

using raytracercanvas = std::vector<std::vector<commontypes::Color>>;

class Canvas final {
   public:
    Canvas(const size_t width, const size_t height) : width_(width), height_(height) {
        std::vector<commontypes::Color> height_vector{height_};
        canvas_.resize(width_, height_vector);

        for (size_t i = 0; i < width_; i++) {
            for (size_t j = 0; j < height_; j++) {
                canvas_[i][j] = commontypes::Color{};
            }
        }
    }

    inline size_t width() { return width_; }
    inline size_t height() { return height_; }
    inline raytracercanvas GetCurrentCanvas() { return canvas_; }

    inline commontypes::Color GetPixel(const size_t x, const size_t y) {
        assert(x >= 0 && x < width_);
        assert(y >= 0 && y < height_);
        return canvas_[x][y];
    }

    inline void WritePixel(const size_t x, const size_t y, commontypes::Color& color) {
        assert(x >= 0 && x < width_);
        assert(y >= 0 && y < height_);
        canvas_[x][y] = color;
    }

   private:
    size_t width_;
    size_t height_;
    raytracercanvas canvas_;
};

#endif  // CANVAS_H
