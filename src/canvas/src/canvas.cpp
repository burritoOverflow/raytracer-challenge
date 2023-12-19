#include "canvas.h"

double canvas::Canvas::Clamp(const double d, const double min, const double max) {
    if (d < min) {
        return 0.0;
    }

    if (d > max) {
        return max;
    }

    return d;
}

std::string canvas::Canvas::WritePPM() const {
    // limit line length to 70 chars
    std::ostringstream out_str;
    /*
        write PPM header with width == 5 and height == 3

        P3
        5 3
        255
    */
    out_str << "P3" << std::endl;
    out_str << width_ << " " << height_ << std::endl;
    out_str << "255" << std::endl;

    // track chars written per line as to not exceed the limit
    const size_t max_line_length = 70;
    size_t num_chars_written = 0;

    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            const commontypes::Color color = GetPixel(x, y);

            // each pixel is written as R G B
            const size_t num_colors = 3;

            for (size_t i = 0; i < num_colors; i++) {
                const int out_val = static_cast<int>(256.0 * Canvas::Clamp(color[i]));
                out_str << out_val;
                num_chars_written += 3;

                // ? this is a tangled mess
                // these antics are implemented to satisfy the book's test suite examples
                if (x == width_ - 1 && i == 2) {
                } else {
                    if (num_chars_written < max_line_length - 4) {
                        out_str << " ";
                        num_chars_written++;
                    } else {
                        out_str << std::endl;
                        num_chars_written = 0;
                    }
                }
            }
        }

        out_str << std::endl;
        num_chars_written = 0;
    }

    return out_str.str();
}
