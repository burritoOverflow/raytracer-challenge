#include <gtest/gtest.h>
#include "color.h"
#include "point.h"
#include "pointlight.h"

TEST(LightingTest, TestPointLightHasIntensityAndPosition) {
    const commontypes::Point position{0, 0, 0};
    const commontypes::Color intensity{1, 1, 1};
    const lighting::PointLight point_light{position, intensity};
    ASSERT_TRUE(point_light.position() == position);
    ASSERT_TRUE(point_light.intensity() == intensity);
}
