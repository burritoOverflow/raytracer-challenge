#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

namespace lighting {
class Material {
   public:
    // default values from book (pg. 85)
    Material()
        : ambient_(0.1),
          diffuse_(0.9),
          specular_(0.9),
          shininess_(200.0),
          color_(commontypes::Color{1, 1, 1}) {}

    Material(const double ambient,
             const double diffuse,
             const double specular,
             const double shininess,
             commontypes::Color color)
        : ambient_(ambient),
          diffuse_(diffuse),
          specular_(specular),
          shininess_(shininess),
          color_(color) {}

    inline double Ambient() const { return ambient_; }
    inline void SetAmbient(const double ambient) { ambient_ = ambient; }

    inline double Diffuse() const { return diffuse_; }
    inline void SetDiffuse(const double diffuse) { diffuse_ = diffuse; }

    inline double Specular() const { return specular_; }
    inline void SetSpecular(const double specular) { specular_ = specular; }

    inline double Shininess() const { return shininess_; }
    inline void SetShininess(const double shininess) { shininess_ = shininess; }

    inline commontypes::Color Color() const { return color_; }
    inline void SetColor(const commontypes::Color& color) { color_ = std::move(color); }

   private:
    // ambient, diffuse, and specular should be between 0-1, shininess should be between 10-200
    double ambient_;
    double diffuse_;
    double specular_;
    double shininess_;
    commontypes::Color color_;
};
}  // namespace lighting

bool operator==(const lighting::Material& m1, const lighting::Material& m2);
bool operator!=(const lighting::Material& m1, const lighting::Material& m2);

#endif  // MATERIAL_H
