#include "camera.h"

// see discussion on p. 102
void scene::Camera::SetPixelSize() {
    // cut the field of view in half
    const double half_view = tan(field_of_view_ / 2);
    // aspect ratio is the ratio of the horizontal size of the canvas to its vertical size
    const double aspect_ratio = static_cast<double>(hsize_) / static_cast<double>(vsize_);

    // horizontal size is greater/or eq than/to the vertical size
    if (aspect_ratio >= 1) {
        // half the width of the canvas
        half_width_ = half_view;
        // half the canvas's height
        half_height_ = half_view / aspect_ratio;
    } else {
        half_width_ = half_view * aspect_ratio;
        half_height_ = half_view;
    }

    // full width of the canvas by the horizontal size (in pixels) of the canvas
    pixel_size_ = (half_width_ * 2) / static_cast<double>(hsize_);
}
