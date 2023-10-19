#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "color.h"
#include "point.h"

namespace lighting {
class PointLight {
   public:
    PointLight() : position_(commontypes::Point{}), intensity_(commontypes::Color{}) {}

    PointLight(const commontypes::Point& position, const commontypes::Color& intensity)
        : position_(position), intensity_(intensity) {}

    inline commontypes::Point position() const { return position_; }
    inline commontypes::Color intensity() const { return intensity_; }

   private:
    commontypes::Point position_;
    commontypes::Color intensity_;
};
}  // namespace lighting

bool operator==(const lighting::PointLight& pl1, const lighting::PointLight& pl2);

#endif  // POINT_LIGHT_H
