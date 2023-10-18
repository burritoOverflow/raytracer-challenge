#ifndef LIGHTING_H
#define LIGHTING_H

#include "color.h"
#include "material.h"
#include "pointlight.h"
#include "vector.h"

namespace lighting {
commontypes::Color Lighting(const Material& material,
                            const PointLight& point_light,
                            const commontypes::Point& point,
                            const commontypes::Vector& eye_vector,
                            const commontypes::Vector& normal_vector);
}

#endif  // LIGHTING_H
