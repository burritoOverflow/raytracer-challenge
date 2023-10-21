#include "material.h"
#include <gtest/gtest.h>
#include "lighting.h"
#include "pointlight.h"
#include "vector.h"

TEST(MaterialTest, TestDefaultMaterial) {
    lighting::Material m{};
    ASSERT_TRUE(m.Color() == commontypes::Color(1, 1, 1));
    ASSERT_DOUBLE_EQ(m.Ambient(), 0.1);
    ASSERT_DOUBLE_EQ(m.Diffuse(), 0.9);
    ASSERT_DOUBLE_EQ(m.Specular(), 0.9);
    ASSERT_DOUBLE_EQ(m.Shininess(), 200.0);
}

TEST(MaterialTest, TestMaterialEquality) {
    lighting::Material m1{};
    lighting::Material m2{};
    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 != m2);
    m1.SetColor(commontypes::Color{1, 2, 3});
    ASSERT_FALSE(m1 == m2);
    ASSERT_TRUE(m1 != m2);
}

// ambient, diffuse, and specular all at full strength (see pg 86)
TEST(MaterialTest, TestLightingWithEyeBetweenLightAndSurface) {
    lighting::Material m{};
    commontypes::Point position{0, 0, 0};
    commontypes::Vector eye_v{0, 0, -1};
    commontypes::Vector normal_v{0, 0, -1};
    lighting::PointLight light{{0, 0, -10}, {1, 1, 1}};
    auto result = lighting::Lighting(std::make_shared<lighting::Material>(m),
                                     std::make_shared<lighting::PointLight>(light), position,
                                     eye_v, normal_v);
    ASSERT_TRUE(result == commontypes::Color(1.9, 1.9, 1.9));
}

// 45 degrees "off" normal (above) see diagram on pg. 86
// specular value falls to effectively 0, accounting for the difference from expected values above
TEST(MaterialTest, TestLightingWithEyeBetweenLightAndSurfaceEyeOffset45Degrees) {
    lighting::Material m{};
    commontypes::Point position{0, 0, 0};
    commontypes::Vector eye_v{0, sqrt(2) / 2, -sqrt(2) / 2};
    commontypes::Vector normal_v{0, 0, -1};
    lighting::PointLight light{{0, 0, -10}, {1, 1, 1}};
    auto result = lighting::Lighting(std::make_shared<lighting::Material>(m),
                                     std::make_shared<lighting::PointLight>(light), position,
                                     eye_v, normal_v);
    ASSERT_TRUE(result == commontypes::Color(1.0, 1.0, 1.0));
}

// eye is directly offset the surfacee, light source is offset 45 degrees (above)
// angle has changed for the light amd the normal vectors, decreasing the diffuse component,
// specular falls to 0 (pg 87)
TEST(MaterialTest, TestLightingWithEyeOppositeSurfaceLightOffset45Degrees) {
    lighting::Material m{};
    commontypes::Point position{0, 0, 0};
    commontypes::Vector eye_v{0, 0, -1};
    commontypes::Vector normal_v{0, 0, -1};
    lighting::PointLight light{{0, 10, -10}, {1, 1, 1}};
    auto result = lighting::Lighting(std::make_shared<lighting::Material>(m),
                                     std::make_shared<lighting::PointLight>(light), position,
                                     eye_v, normal_v);
    ASSERT_TRUE(result == commontypes::Color(0.7364, 0.7364, 0.7364));
}

// ambient + diffuse same as previous test, but specular in full-strength (eye moved directly in
// path of reflection vector)
TEST(MaterialTest, TestLightingWithEyeInThePathOfTheReflectionVector) {
    lighting::Material m{};
    commontypes::Point position{0, 0, 0};
    commontypes::Vector eye_v{0, -sqrt(2) / 2, -sqrt(2) / 2};
    commontypes::Vector normal_v{0, 0, -1};
    lighting::PointLight light{{0, 10, -10}, {1, 1, 1}};
    auto result = lighting::Lighting(std::make_shared<lighting::Material>(m),
                                     std::make_shared<lighting::PointLight>(light), position,
                                     eye_v, normal_v);
    ASSERT_TRUE(result == commontypes::Color(1.6364, 1.6364, 1.6364));
}

// light moved behind the surface, leaving only the ambient component
TEST(MaterialTest, TestLightingWithLightBehindTheSurface) {
    lighting::Material m{};
    commontypes::Point position{0, 0, 0};
    commontypes::Vector eye_v{0, 0, -1};
    commontypes::Vector normal_v{0, 0, -1};
    lighting::PointLight light{{0, 0, 10}, {1, 1, 1}};
    auto result = lighting::Lighting(std::make_shared<lighting::Material>(m),
                                     std::make_shared<lighting::PointLight>(light), position,
                                     eye_v, normal_v);
    ASSERT_TRUE(result == commontypes::Color(0.1, 0.1, 0.1));
}