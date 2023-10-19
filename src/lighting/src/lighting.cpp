#include "lighting.h"

commontypes::Color lighting::Lighting(const Material& material,
                                      const PointLight& point_light,
                                      const commontypes::Point& point,
                                      const commontypes::Vector& eye_vector,
                                      const commontypes::Vector& normal_vector) {
    // surface color with the light's color/intensity
    const auto effective_color = material.Color() * point_light.intensity();

    // direction to the light source
    commontypes::Vector light_vector = (point_light.position() - point).Normalize();

    // ambient color contribution
    const auto ambient = commontypes::Color{effective_color * material.Ambient()};

    // represents the cosine of the angle between the light vector and the normal vector
    // negative number means the light is on the other side of the surface
    const double light_dot_normal = light_vector.Dot(normal_vector);
    commontypes::Color diffuse, specular;
    if (light_dot_normal < 0.0) {
        // other side of the surface
        diffuse = commontypes::Color::Black();
        specular = commontypes::Color::Black();
    } else {
        // diffuse contribution
        diffuse = commontypes::Color{effective_color * material.Diffuse() * light_dot_normal};

        // reflect_dot_eye is the cosine of the angle between the reflection
        // vector and the eye vector. A negative number means the light reflects
        // away from the eye
        commontypes::Vector reflect_v = -light_vector.Reflect(normal_vector);
        const double reflect_dot_eye = reflect_v.Dot(eye_vector);

        if (reflect_dot_eye <= 0.0) {
            specular = commontypes::Color::Black();
        } else {
            // contribute specular contribution
            const double factor = pow(reflect_dot_eye, material.Shininess());
            specular = commontypes::Color{point_light.intensity() * material.Specular() * factor};
        }
    }
    return commontypes::Color{ambient + diffuse + specular};
}