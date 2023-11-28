#include "material.h"
#include <gtest/gtest.h>
#include <memory>
#include "color.h"
#include "identitymatrix.h"
#include "lighting.h"
#include "pattern.h"
#include "point.h"
#include "pointlight.h"
#include "stripepattern.h"
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
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);
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
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);
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
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);
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
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);
    ASSERT_TRUE(result == commontypes::Color(1.6364, 1.6364, 1.6364));
}

// light moved behind the surface, leaving only the ambient component
TEST(MaterialTest, TestLightingWithLightBehindTheSurface) {
    lighting::Material m{};
    commontypes::Point position{0, 0, 0};
    commontypes::Vector eye_v{0, 0, -1};
    commontypes::Vector normal_v{0, 0, -1};
    lighting::PointLight light{{0, 0, 10}, {1, 1, 1}};
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);
    ASSERT_TRUE(result == commontypes::Color(0.1, 0.1, 0.1));
}

TEST(MaterialTest, TestLightingWithSurfaceInShadow) {
    lighting::Material m{};
    commontypes::Point position{0, 0, 0};
    commontypes::Vector eye_v{0, 0, -1};
    commontypes::Vector normal_v{0, 0, -1};
    lighting::PointLight light{{0, 0, -10}, {1, 1, 1}};
    const bool in_shadow{true};
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v, in_shadow);
    ASSERT_TRUE(result == commontypes::Color(0.1, 0.1, 0.1));
}

TEST(MaterialTest, TestLightingWithPatternApplied) {
    lighting::Material m{};
    m.SetPattern(std::make_shared<pattern::StripePattern>(
        pattern::StripePattern{commontypes::Color{1, 1, 1}, commontypes::Color{0, 0, 0}}));
    m.SetAmbient(1);
    m.SetDiffuse(0);
    m.SetSpecular(0);
    const commontypes::Vector eye_v{0, 0, -1};
    const commontypes::Vector normal_v{0, 0, -1};
    const lighting::PointLight light{commontypes::Point{0, 0, -10}, commontypes::Color{1, 1, 1}};

    const commontypes::Color c1 =
        lighting::Lighting(std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
                           std::make_shared<lighting::PointLight>(light),
                           commontypes::Point{0.9, 0, 0}, eye_v, normal_v);

    const commontypes::Color c2 =
        lighting::Lighting(std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
                           std::make_shared<lighting::PointLight>(light),
                           commontypes::Point{1.1, 0, 0}, eye_v, normal_v);

    ASSERT_TRUE(c1 == commontypes::Color(1, 1, 1));
    ASSERT_TRUE(c2 == commontypes::Color(0, 0, 0));
}