#include "pointlight.h"

bool operator==(const lighting::PointLight& pl1, const lighting::PointLight& pl2) {
    return pl1.position() == pl2.position() && pl1.intensity() == pl2.intensity();
}
