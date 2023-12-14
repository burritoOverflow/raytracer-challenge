#ifndef MATERIAL_H
#define MATERIAL_H

#include <cstddef>
#include <memory>
#include "color.h"

namespace pattern {
class Pattern;
}

namespace lighting {
class Material {
   public:
    // default values from book (pg. 85)
    Material()
        : ambient_(0.1),
          diffuse_(0.9),
          specular_(0.9),
          shininess_(200.0),
          reflective_(0.0),  // reflective is 0 surface is completely non-reflective; 1 is a mirror
          color_(commontypes::Color{1, 1, 1}),
          pattern_ptr_(nullptr)  // Pattern is optional for Materials
    {}

    Material(Material&& material) noexcept
        : ambient_(material.ambient_),
          diffuse_(material.diffuse_),
          specular_(material.specular_),
          shininess_(material.shininess_),
          reflective_(material.reflective_),
          color_(material.color_) {}

    Material(Material const& material) = default;

    Material(const double ambient,
             const double diffuse,
             const double specular,
             const double shininess,
             const double reflective,
             commontypes::Color color)
        : ambient_(ambient),
          diffuse_(diffuse),
          specular_(specular),
          shininess_(shininess),
          reflective_(reflective),
          color_(color) {}

    inline double Ambient() const { return ambient_; }
    inline void SetAmbient(const double ambient) { ambient_ = ambient; }

    inline double Diffuse() const { return diffuse_; }
    inline void SetDiffuse(const double diffuse) { diffuse_ = diffuse; }

    inline double Specular() const { return specular_; }
    inline void SetSpecular(const double specular) { specular_ = specular; }

    inline double Shininess() const { return shininess_; }
    inline void SetShininess(const double shininess) { shininess_ = shininess; }

    inline double Reflective() const { return reflective_; }
    inline void SetReflective(const double reflective) { reflective_ = reflective; }

    inline commontypes::Color Color() const { return color_; }
    inline void SetColor(const commontypes::Color& color) { color_ = color; }

    inline bool HasPattern() const { return pattern_ptr_ != nullptr; }
    inline std::shared_ptr<pattern::Pattern> Pattern() { return pattern_ptr_; }
    inline void SetPattern(const std::shared_ptr<pattern::Pattern>& pattern_ptr) {
        pattern_ptr_ = pattern_ptr;
    }

    Material operator=(const Material& other) = delete;

    inline Material& operator=(Material&& other) noexcept {
        if (this == &other)
            return *this;

        ambient_ = other.ambient_;
        diffuse_ = other.diffuse_;
        specular_ = other.specular_;
        shininess_ = other.shininess_;
        color_ = other.color_;

        return *this;
    }

   private:
    // ambient, diffuse, and specular should be between 0-1, shininess should be between 10-200
    double ambient_;
    double diffuse_;
    double specular_;
    double shininess_;
    double reflective_;
    commontypes::Color color_;
    std::shared_ptr<pattern::Pattern> pattern_ptr_;

    friend class MaterialBuilder;
};

class MaterialBuilder {
   public:
    MaterialBuilder& WithAmbient(const double ambient) {
        this->m_.ambient_ = ambient;
        return *this;
    }

    MaterialBuilder& WithDiffuse(const double diffuse) {
        this->m_.diffuse_ = diffuse;
        return *this;
    }

    MaterialBuilder& WithSpecular(const double specular) {
        this->m_.specular_ = specular;
        return *this;
    }

    MaterialBuilder& WithShininess(const double shininess) {
        this->m_.shininess_ = shininess;
        return *this;
    }

    MaterialBuilder& WithReflective(const double reflective) {
        this->m_.reflective_ = reflective;
        return *this;
    }

    MaterialBuilder& WithColor(const commontypes::Color& color) {
        this->m_.color_ = color;
        return *this;
    }

    MaterialBuilder& WithPatternPtr(const std::shared_ptr<pattern::Pattern>& pattern_ptr) {
        this->m_.pattern_ptr_ = pattern_ptr;
        return *this;
    }

    Material& Build() { return m_; }

    operator Material() { return std::move(m_); }

   private:
    Material m_{};
};
}  // namespace lighting

bool operator==(const lighting::Material& m1, const lighting::Material& m2);
bool operator!=(const lighting::Material& m1, const lighting::Material& m2);

#endif  // MATERIAL_H
