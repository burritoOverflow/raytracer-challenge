#include "material.h"

bool operator==(const lighting::Material& m1, const lighting::Material& m2) {
    if (!utility::NearEquals(m1.Ambient(), m2.Ambient())) {
        return false;
    }

    if (!utility::NearEquals(m1.Diffuse(), m2.Diffuse())) {
        return false;
    }

    if (!utility::NearEquals(m1.Specular(), m2.Specular())) {
        return false;
    }

    if (!utility::NearEquals(m1.Shininess(), m2.Shininess())) {
        return false;
    }

    if (m1.Color() != m2.Color()) {
        return false;
    }

    return true;
}

bool operator!=(const lighting::Material& m1, const lighting::Material& m2) {
    return !(m1 == m2);
}
