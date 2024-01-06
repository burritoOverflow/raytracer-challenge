#include "material.h"
#include <gtest/gtest.h>
#include <memory>
#include "color.h"
#include "identitymatrix.h"
#include "lighting.h"
#include "point.h"
#include "pointlight.h"
#include "stripepattern.h"
#include "vector.h"

TEST(MaterialTest, TestMaterialMoveCtor) {
    const auto color_a = commontypes::Color{0, 1, 0};
    const auto color_b = commontypes::Color{0, 0, 0};

    auto pattern_ptr =
        std::make_shared<pattern::StripePattern>(pattern::StripePattern{color_a, color_b});

    const double expected_ambience = 0.5;
    const double expected_specular = 0.33;
    const double zero_val = 0.0;
    const auto im = commontypes::IdentityMatrix();

    lighting::Material material = lighting::MaterialBuilder()
                                      .WithAmbient(expected_ambience)
                                      .WithSpecular(expected_specular)
                                      .WithPatternPtr(std::move(pattern_ptr));

    // verify moved to
    const lighting::Material material2 = std::move(material);
    ASSERT_DOUBLE_EQ(material2.Ambient(), expected_ambience);
    ASSERT_DOUBLE_EQ(material2.Specular(), expected_specular);

    const auto material_pattern_ptr =
        dynamic_cast<pattern::StripePattern*>(material2.Pattern().get());
    ASSERT_TRUE(material_pattern_ptr->ColorA() == color_a);
    ASSERT_TRUE(material_pattern_ptr->ColorB() == color_b);
    ASSERT_TRUE(material_pattern_ptr->GetPatternTransform() == im);

    // verify moved from (we're aware of the warning; intentional)
    ASSERT_DOUBLE_EQ(material.Ambient(), zero_val);
    ASSERT_DOUBLE_EQ(material.Diffuse(), zero_val);
    ASSERT_DOUBLE_EQ(material.Specular(), zero_val);
    ASSERT_DOUBLE_EQ(material.Shininess(), zero_val);
    ASSERT_DOUBLE_EQ(material.Reflective(), zero_val);
    ASSERT_DOUBLE_EQ(material.RefractiveIndex(), zero_val);
    ASSERT_TRUE(material.Color() == commontypes::Color());
    ASSERT_FALSE(material.HasPattern());
    ASSERT_EQ(material.Pattern(), nullptr);
}

TEST(MaterialTest, TestMaterialBuilderAssignment) {
    commontypes::Color color{1, 0, 1};
    const double ambient = 0.6;
    const double diffuse = 0.1;
    const double specular = 0.3;
    const double shininess = 0.4;
    const double reflective = 0.2;
    const double transparency = 0.5;
    const double refractive_index = 0.8;

    const auto material = lighting::MaterialBuilder{}
                              .WithAmbient(ambient)
                              .WithDiffuse(diffuse)
                              .WithShininess(shininess)
                              .WithSpecular(specular)
                              .WithReflective(reflective)
                              .WithTransparency(transparency)
                              .WithRefractiveIndex(refractive_index)
                              .WithColor(color)
                              .Build();

    ASSERT_TRUE(material.Ambient() == ambient);
    ASSERT_TRUE(material.Diffuse() == diffuse);
    ASSERT_TRUE(material.Shininess() == shininess);
    ASSERT_TRUE(material.Specular() == specular);
    ASSERT_TRUE(material.Reflective() == reflective);
    ASSERT_TRUE(material.Transparency() == transparency);
    ASSERT_TRUE(material.RefractiveIndex() == refractive_index);
    ASSERT_TRUE(material.Color() == color);
}

TEST(MaterialTest, TestMaterialBuilderInvokesDefaultCtor) {
    const commontypes::Color color{1, 0, 1};
    const double ambient = 0.6;
    const double reflective = 0.2;

    const lighting::Material default_material{};
    const lighting::Material material = lighting::MaterialBuilder()
                                            .WithAmbient(ambient)
                                            .WithReflective(reflective)
                                            .WithColor(color);

    // ensure that the default values are provided by the ctor (implicitly invoked)
    ASSERT_TRUE(material.Ambient() == ambient);
    ASSERT_FALSE(material.Ambient() == default_material.Ambient());
    ASSERT_TRUE(material.Reflective() == reflective);
    ASSERT_FALSE(material.Reflective() == default_material.Reflective());
    ASSERT_TRUE(material.Color() == color);
    ASSERT_FALSE(material.Color() == default_material.Color());

    ASSERT_TRUE(material.Specular() == default_material.Specular());
    ASSERT_TRUE(material.Shininess() == default_material.Shininess());
    ASSERT_TRUE(material.Specular() == default_material.Specular());
    ASSERT_TRUE(material.Diffuse() == default_material.Diffuse());
    ASSERT_TRUE(material.Transparency() == default_material.Transparency());
    ASSERT_TRUE(material.RefractiveIndex() == default_material.RefractiveIndex());
}

TEST(MaterialTest, TestMaterialBuilderCallOperator) {
    const double exp_ambient = 1.0;
    const double exp_specular = 0.5;

    const auto color_a = commontypes::Color{0, 1, 0};
    const auto color_b = commontypes::Color{0, 0, 0};

    const auto pattern_ptr =
        std::make_shared<pattern::StripePattern>(pattern::StripePattern{color_a, color_b});

    const lighting::Material material = lighting::MaterialBuilder()
                                            .WithAmbient(exp_ambient)
                                            .WithSpecular(exp_specular)
                                            .WithPatternPtr(pattern_ptr);

    EXPECT_DOUBLE_EQ(material.Ambient(), exp_ambient);
    EXPECT_DOUBLE_EQ(material.Specular(), exp_specular);
    EXPECT_EQ(material.Pattern(), pattern_ptr);
}

// as above; description summarizes changes
TEST(MaterialTest, TestMaterialBuilderCallOperatorWithMovedSharedPtrMember) {
    const double exp_ambient = 1.0;
    const double exp_specular = 0.5;
    const auto color_a = commontypes::Color{0, 1, 0};
    const auto color_b = commontypes::Color{0, 0, 0};

    auto pattern_ptr =
        std::make_shared<pattern::StripePattern>(pattern::StripePattern{color_a, color_b});

    const lighting::Material material = lighting::MaterialBuilder()
                                            .WithAmbient(exp_ambient)
                                            .WithSpecular(exp_specular)
                                            .WithPatternPtr(std::move(pattern_ptr));

    ASSERT_TRUE(material.HasPattern());
    ASSERT_EQ(pattern_ptr, nullptr);

    const auto material_pattern_ptr =
        dynamic_cast<pattern::StripePattern*>(material.Pattern().get());

    EXPECT_DOUBLE_EQ(material.Ambient(), exp_ambient);
    EXPECT_DOUBLE_EQ(material.Specular(), exp_specular);
    ASSERT_TRUE(material_pattern_ptr->ColorA() == color_a);
    ASSERT_TRUE(material_pattern_ptr->ColorB() == color_b);
}

TEST(MaterialTest, TestDefaultMaterial) {
    const lighting::Material m{};
    ASSERT_TRUE(m.Color() == commontypes::Color(1, 1, 1));
    ASSERT_DOUBLE_EQ(m.Ambient(), 0.1);
    ASSERT_DOUBLE_EQ(m.Diffuse(), 0.9);
    ASSERT_DOUBLE_EQ(m.Specular(), 0.9);
    ASSERT_DOUBLE_EQ(m.Shininess(), 200.0);
    ASSERT_DOUBLE_EQ(m.Reflective(), 0.0);
    ASSERT_DOUBLE_EQ(m.Transparency(), 0.0);
    ASSERT_DOUBLE_EQ(m.RefractiveIndex(), 1.0);
}

TEST(MaterialTest, TestMaterialEquality) {
    lighting::Material m1{};
    const lighting::Material m2{};

    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 != m2);

    m1.SetColor(commontypes::Color{1, 2, 3});
    ASSERT_FALSE(m1 == m2);
    ASSERT_TRUE(m1 != m2);
}

// ambient, diffuse, and specular all at full strength (see pg 86)
TEST(MaterialTest, TestLightingWithEyeBetweenLightAndSurface) {
    const lighting::Material m{};
    const commontypes::Point position{0, 0, 0};
    const commontypes::Vector eye_v{0, 0, -1};
    const commontypes::Vector normal_v{0, 0, -1};
    const lighting::PointLight light{commontypes::Point{0, 0, -10}, commontypes::Color{1, 1, 1}};
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);

    ASSERT_TRUE(result == commontypes::Color(1.9, 1.9, 1.9));
}

// 45 degrees "off" normal (above) see diagram on pg. 86
// specular value falls to effectively 0, accounting for the difference from expected values above
TEST(MaterialTest, TestLightingWithEyeBetweenLightAndSurfaceEyeOffset45Degrees) {
    const lighting::Material m{};
    const commontypes::Point position{0, 0, 0};
    const commontypes::Vector eye_v{0, sqrt(2) / 2, -sqrt(2) / 2};
    const commontypes::Vector normal_v{0, 0, -1};
    const lighting::PointLight light{commontypes::Point{0, 0, -10}, commontypes::Color{1, 1, 1}};
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);

    ASSERT_TRUE(result == commontypes::Color(1.0, 1.0, 1.0));
}

// eye is directly offset the surface light source is offset 45 degrees (above)
// angle has changed for the light amd the normal vectors, decreasing the diffuse component,
// specular falls to 0 (pg 87)
TEST(MaterialTest, TestLightingWithEyeOppositeSurfaceLightOffset45Degrees) {
    const lighting::Material m{};
    const commontypes::Point position{0, 0, 0};
    const commontypes::Vector eye_v{0, 0, -1};
    const commontypes::Vector normal_v{0, 0, -1};
    const lighting::PointLight light{commontypes::Point{0, 10, -10}, commontypes::Color{1, 1, 1}};
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);

    ASSERT_TRUE(result == commontypes::Color(0.7364, 0.7364, 0.7364));
}

// ambient + diffuse same as previous test, but specular in full-strength (eye moved directly in
// path of reflection vector)
TEST(MaterialTest, TestLightingWithEyeInThePathOfTheReflectionVector) {
    const lighting::Material m{};
    const commontypes::Point position{0, 0, 0};
    const commontypes::Vector eye_v{0, -sqrt(2) / 2, -sqrt(2) / 2};
    const commontypes::Vector normal_v{0, 0, -1};
    const lighting::PointLight light{commontypes::Point{0, 10, -10}, commontypes::Color{1, 1, 1}};
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);

    ASSERT_TRUE(result == commontypes::Color(1.6364, 1.6364, 1.6364));
}

// light moved behind the surface, leaving only the ambient component
TEST(MaterialTest, TestLightingWithLightBehindTheSurface) {
    const lighting::Material m{};
    const commontypes::Point position{0, 0, 0};
    const commontypes::Vector eye_v{0, 0, -1};
    const commontypes::Vector normal_v{0, 0, -1};
    const lighting::PointLight light{commontypes::Point{0, 0, 10}, commontypes::Color{1, 1, 1}};
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v);

    ASSERT_TRUE(result == commontypes::Color(0.1, 0.1, 0.1));
}

TEST(MaterialTest, TestLightingWithSurfaceInShadow) {
    const lighting::Material m{};
    const commontypes::Point position{0, 0, 0};
    const commontypes::Vector eye_v{0, 0, -1};
    const commontypes::Vector normal_v{0, 0, -1};
    const lighting::PointLight light{commontypes::Point{0, 0, -10}, commontypes::Color{1, 1, 1}};
    const bool in_shadow{true};
    auto result = lighting::Lighting(
        std::make_shared<lighting::Material>(m), commontypes::IdentityMatrix{},
        std::make_shared<lighting::PointLight>(light), position, eye_v, normal_v, in_shadow);

    ASSERT_TRUE(result == commontypes::Color(0.1, 0.1, 0.1));
}

TEST(MaterialTest, TestLightingWithPatternApplied) {
    const auto pattern_ptr = std::make_shared<pattern::StripePattern>(
        pattern::StripePattern{commontypes::Color{1, 1, 1}, commontypes::Color{0, 0, 0}});

    lighting::Material m = lighting::MaterialBuilder()
                               .WithAmbient(1)
                               .WithDiffuse(0)
                               .WithSpecular(0)
                               .WithPatternPtr(pattern_ptr)
                               .Build();

    const commontypes::Vector eye_v{0, 0, -1};
    const commontypes::Vector normal_v{0, 0, -1};
    lighting::PointLight light{commontypes::Point{0, 0, -10}, commontypes::Color{1, 1, 1}};

    const auto material_ptr = std::make_shared<lighting::Material>(std::move(m));
    const auto lighting_ptr = std::make_shared<lighting::PointLight>(light);
    const auto identity_matrix = commontypes::IdentityMatrix{};

    const commontypes::Color c1 =
        lighting::Lighting(material_ptr, identity_matrix, lighting_ptr,
                           commontypes::Point{0.9, 0, 0}, eye_v, normal_v);

    const commontypes::Color c2 =
        lighting::Lighting(material_ptr, identity_matrix, lighting_ptr,
                           commontypes::Point{1.1, 0, 0}, eye_v, normal_v);

    ASSERT_TRUE(c1 == commontypes::Color(1, 1, 1));
    ASSERT_TRUE(c2 == commontypes::Color(0, 0, 0));
}