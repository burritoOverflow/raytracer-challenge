#ifndef CAMERA_H
#define CAMERA_H

#include "canvas.h"
#include "identitymatrix.h"
#include "ray.h"
#include "world.h"

namespace scene {
class Camera {
   public:
    Camera(const size_t hsize, const size_t vsize, const double field_of_view)
        : hsize_(hsize),
          vsize_(vsize),
          field_of_view_(field_of_view),
          transform_(commontypes::IdentityMatrix{}) {
        SetPixelSize();
    }

    size_t hsize() const { return hsize_; }
    size_t vsize() const { return vsize_; }
    double field_of_view() const { return field_of_view_; }
    commontypes::Matrix transform() const { return transform_; }
    double pixel_size() const { return pixel_size_; }

    inline void SetTransform(const commontypes::Matrix& transform_matrix) {
        this->transform_ = transform_matrix;
    }

    // computes the world coords for the center of the given pixel and
    // construct a ray that passes through that point
    commontypes::Ray RayForPixel(const size_t px, const size_t py) const;

    // render the contents of the "world" to a Canvas
    Canvas Render(scene::World& world) const;

   private:
    size_t
        hsize_;  // horizontal size (in pixels of the canvas that the picture will be rendered to)
    size_t vsize_;          // as above, but vertical
    double field_of_view_;  // angle that describes how much the camera can see
    commontypes::Matrix
        transform_;  // matrix describing how the world should be oriented relative to the camera
    double half_width_;
    double half_height_;
    double pixel_size_;

    // calculate (and set) the size of the pixels on the Canvas (in world-space units)
    void SetPixelSize();
};
}  // namespace scene
#endif  // CAMERA_H
