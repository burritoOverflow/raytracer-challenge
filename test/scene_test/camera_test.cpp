#include "camera.h"
#include <gtest/gtest.h>
#include "color.h"
#include "rotationmatrix.h"
#include "translationmatrix.h"
#include "viewtransform.h"
#include "world.h"

TEST(CameraTest, TestConstructingACamera) {
    const size_t hsize = 160;
    const size_t vsize = 120;
    const double field_of_view = M_PI_2;
    scene::Camera c{hsize, vsize, field_of_view};
    ASSERT_EQ(c.hsize(), hsize);
    ASSERT_EQ(c.vsize(), vsize);
    ASSERT_DOUBLE_EQ(c.field_of_view(), field_of_view);
    ASSERT_TRUE(c.transform() == commontypes::IdentityMatrix());
}

TEST(CameraTest, TestPixelSizeHorizontalCanvas) {
    scene::Camera c{200, 125, M_PI_2};
    ASSERT_DOUBLE_EQ(c.pixel_size(), 0.01);
}

TEST(CameraTest, TestPixelSizeVerticalCanvas) {
    scene::Camera c{125, 200, M_PI_2};
    ASSERT_DOUBLE_EQ(c.pixel_size(), 0.01);
}

TEST(CameraTest, TestConstructingRayThroughCenterOfCanvas) {
    scene::Camera c{201, 101, M_PI_2};
    auto r = c.RayForPixel(100, 50);
    ASSERT_TRUE(r.origin() == commontypes::Point(0, 0, 0));
    ASSERT_TRUE(r.direction() == commontypes::Vector(0, 0, -1));
}

TEST(CameraTest, TestConstructingRayThroughCornerOfCanvas) {
    scene::Camera c{201, 101, M_PI_2};
    auto r = c.RayForPixel(0, 0);
    ASSERT_TRUE(r.origin() == commontypes::Point(0, 0, 0));
    ASSERT_TRUE(r.direction() == commontypes::Vector(0.66519, 0.33259, -0.66851));
}

TEST(CameraTest, TestConstructingRayWhenCameraIsTransformed) {
    scene::Camera c{201, 101, M_PI_2};
    const auto translation =
        commontypes::RotationMatrixY{M_PI_4} * commontypes::TranslationMatrix{0, -2, 5};
    c.SetTransform(translation);
    auto r = c.RayForPixel(100, 50);
    ASSERT_TRUE(r.origin() == commontypes::Point(0, 2, -5));
    ASSERT_TRUE(r.direction() == commontypes::Vector(sqrt(2) / 2, 0, -sqrt(2) / 2));
}

TEST(CameraTest, TestRenderingWorldWithCamera) {
    scene::World world = scene::World::DefaultWorld();
    scene::Camera camera{11, 11, M_PI_2};
    const commontypes::Point from{0, 0, -5};
    const commontypes::Point to{0, 0, 0};
    const commontypes::Vector up{0, 1, 0};
    const commontypes::ViewTransform t{from, to, up};
    camera.SetTransform(t);
    Canvas image = camera.Render(world);
    const commontypes::Color pixel_at = image.GetPixel(5, 5);
    ASSERT_TRUE(pixel_at == commontypes::Color(0.38066, 0.47583, 0.2855));
}
