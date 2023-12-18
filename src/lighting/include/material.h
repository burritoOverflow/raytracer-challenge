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
          refractive_index_(1.0),
          transparency_(0.0),
          color_(commontypes::Color{1, 1, 1}),
          pattern_ptr_(nullptr)  // Pattern is optional for Materials
    {}

    Material(Material&& material) noexcept
        : ambient_(material.ambient_),
          diffuse_(material.diffuse_),
          specular_(material.specular_),
          shininess_(material.shininess_),
          reflective_(material.reflective_),
          transparency_(material.transparency_),
          refractive_index_(material.refractive_index_),
          color_(material.color_) {}

    Material(Material const& material) = default;

    Material(const double ambient,
             const double diffuse,
             const double specular,
             const double shininess,
             const double reflective,
             const double transparency,
             const double refractive_index,
             commontypes::Color color)
        : ambient_(ambient),
          diffuse_(diffuse),
          specular_(specular),
          shininess_(shininess),
          reflective_(reflective),
          transparency_(transparency),
          refractive_index_(refractive_index),
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

    inline double Transparency() const { return transparency_; }
    inline void SetTransparency(const double transparency) { transparency_ = transparency; }

    inline double RefractiveIndex() const { return refractive_index_; }
    inline void SetRefractiveIndex(const double refractive_index) {
        refractive_index_ = refractive_index;
    }

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
        transparency_ = other.transparency_;
        refractive_index_ = other.refractive_index_;
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
    double transparency_;
    double refractive_index_;  // determines the degree to which light will bend when entering or
                               // exiting the material (see pg. 149)
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

    MaterialBuilder& WithTransparency(const double transparency) {
        this->m_.transparency_ = transparency;
        return *this;
    }

    MaterialBuilder& WithRefractiveIndex(const double refractive_index) {
        this->m_.refractive_index_ = refractive_index;
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

    // WARNING: attempting to build using the call operator while setting the pattern_ptr results
    // in the pattern_ptr being NULL; call `Build` instead. considering this, this should be
    // removed to avoid the issue, but it's worth investigating
    operator Material() { return std::move(m_); }

   private:
    Material m_{};
};
}  // namespace lighting

bool operator==(const lighting::Material& m1, const lighting::Material& m2);
bool operator!=(const lighting::Material& m1, const lighting::Material& m2);

#endif  // MATERIAL_H
