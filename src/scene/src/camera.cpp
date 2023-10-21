#include "camera.h"

void scene::Camera::GetPixelSize() {
    const double half_view = tan(field_of_view_ / 2);
    const double aspect = static_cast<double>(hsize_) / static_cast<double>(vsize_);
    if (aspect >= 1) {
        half_width_ = half_view;
        half_height_ = half_view / aspect;
    } else {
        half_width_ = half_view * aspect;
        half_height_ = half_view;
    }
    pixel_size_ = (half_width_ * 2) / hsize_;
}
