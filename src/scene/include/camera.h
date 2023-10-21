#ifndef CAMERA_H
#define CAMERA_H

#include "identitymatrix.h"

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
