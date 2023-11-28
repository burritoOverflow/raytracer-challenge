#ifndef LIGHTING_H
#define LIGHTING_H

#include <memory>
#include "color.h"
#include "material.h"
#include "matrix.h"
#include "pointlight.h"
#include "vector.h"

namespace lighting {
commontypes::Color Lighting(
    const std::shared_ptr<Material>& material_ptr,
    const commontypes::Matrix& object_transform,  // a Shape's transformation matrix
    const std::shared_ptr<PointLight>& point_light_ptr,
    const commontypes::Point& point,
    const commontypes::Vector& eye_vector,
    const commontypes::Vector& normal_vector,
    bool in_shadow = false);
}

#endif  // LIGHTING_H
