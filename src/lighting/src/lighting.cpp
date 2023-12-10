#include "lighting.h"
#include "color.h"
#include "pattern.h"

commontypes::Color lighting::Lighting(const std::shared_ptr<Material>& material_ptr,
                                      const commontypes::Matrix& object_transform,
                                      const std::shared_ptr<PointLight>& point_light_ptr,
                                      const commontypes::Point& point,
                                      const commontypes::Vector& eye_vector,
                                      const commontypes::Vector& normal_vector,
                                      const bool in_shadow) {
    // surface color with the light's color/intensity
    auto material = *material_ptr;
    const auto point_light = *point_light_ptr;

    // with no pattern present, use the Material's color.
    commontypes::Color color = material.Color();

    // otherwise, usee the material's pattern at the given Shape
    if (material.HasPattern()) {
        color = material.Pattern()->PatternAtShape(object_transform, point);
    }

    const auto effective_color = color * point_light.intensity();

    // direction to the light source
    const commontypes::Vector light_vector = (point_light.position() - point).Normalize();

    // ambient color contribution
    const auto ambient = commontypes::Color{effective_color * material.Ambient()};

    // when shadow, ignore the contributions of diffuse and specular (only ambient contributes)
    if (in_shadow) {
        return commontypes::Color{ambient};
    }

    // represents the cosine of the angle between the light vector and the normal vector
    // negative number means the light is on the other side of the surface
    const double light_dot_normal = light_vector.Dot(normal_vector);

    commontypes::Color diffuse, specular;

    if (light_dot_normal < 0.0) {
        // other side of the surface
        diffuse = commontypes::Color::MakeBlack();
        specular = commontypes::Color::MakeBlack();
    } else {
        // diffuse contribution
        diffuse = commontypes::Color{effective_color * material.Diffuse() * light_dot_normal};

        // reflect_dot_eye is the cosine of the angle between the reflection
        // vector and the eye vector. A negative number means the light reflects
        // away from the eye
        commontypes::Vector reflect_v = -light_vector.Reflect(normal_vector);
        const double reflect_dot_eye = reflect_v.Dot(eye_vector);

        if (reflect_dot_eye <= 0.0) {
            specular = commontypes::Color::MakeBlack();
        } else {
            // contribute specular contribution
            const double factor = pow(reflect_dot_eye, material.Shininess());
            specular = commontypes::Color{point_light.intensity() * material.Specular() * factor};
        }
    }

    return commontypes::Color{ambient + diffuse + specular};
}