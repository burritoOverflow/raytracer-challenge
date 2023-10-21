#include "camera.h"

commontypes::Ray scene::Camera::RayForPixel(const size_t px, const size_t py) const {
    // offset from edge of the canvas to the pixel's center
    const double x_offset = (static_cast<double>(px) + 0.5) * pixel_size_;
    const double y_offset = (static_cast<double>(py) + 0.5) * pixel_size_;

    // untransformed coords of the pixel in world space
    // recall that camera looks toward -z, so +x is "left"
    const double world_x = this->half_width_ - x_offset;
    const double world_y = this->half_height_ - y_offset;

    commontypes::Matrix transform_inverse = this->transform_.Inverse();

    commontypes::Point pixel =
        commontypes::Point(transform_inverse * commontypes::Point{world_x, world_y, -1});
    const commontypes::Point origin =
        commontypes::Point(transform_inverse * commontypes::Point{0, 0, 0});
    const commontypes::Vector direction = (pixel - origin).Normalize();

    return {origin, direction};
}

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
