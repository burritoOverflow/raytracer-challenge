#include "material.h"
#include <gtest/gtest.h>

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
