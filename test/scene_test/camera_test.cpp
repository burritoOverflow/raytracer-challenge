#include "camera.h"
#include <gtest/gtest.h>
#include "rotationmatrix.h"
#include "translationmatrix.h"

TEST(CameraTest, TestConstructingACamera) {
    const size_t hsize = 160;
    const size_t vsize = 120;
    const double field_of_view = M_PI / 2;
    scene::Camera c{hsize, vsize, field_of_view};
    ASSERT_EQ(c.hsize(), hsize);
    ASSERT_EQ(c.vsize(), vsize);
    ASSERT_DOUBLE_EQ(c.field_of_view(), field_of_view);
    ASSERT_TRUE(c.transform() == commontypes::IdentityMatrix());
}

TEST(CameraTest, TestPixelSizeHorizontalCanvas) {
    scene::Camera c{200, 125, M_PI / 2};
    ASSERT_DOUBLE_EQ(c.pixel_size(), 0.01);
}

TEST(CameraTest, TestPixelSizeVerticalCanvas) {
    scene::Camera c{125, 200, M_PI / 2};
    ASSERT_DOUBLE_EQ(c.pixel_size(), 0.01);
}

TEST(CameraTest, TestConstructingRayThroughCenterOfCanvas) {
    scene::Camera c{201, 101, M_PI / 2};
    auto r = c.RayForPixel(100, 50);
    ASSERT_TRUE(r.origin() == commontypes::Point(0, 0, 0));
    ASSERT_TRUE(r.direction() == commontypes::Vector(0, 0, -1));
}

TEST(CameraTest, TestConstructingRayThroughCornerOfCanvas) {
    scene::Camera c{201, 101, M_PI / 2};
    auto r = c.RayForPixel(0, 0);
    ASSERT_TRUE(r.origin() == commontypes::Point(0, 0, 0));
    ASSERT_TRUE(r.direction() == commontypes::Vector(0.66519, 0.33259, -0.66851));
}

TEST(CameraTest, TestConstructingRayWhenCameraIsTransformed) {
    scene::Camera c{201, 101, M_PI / 2};
    const auto translation =
        commontypes::RotationMatrixY{M_PI_4} * commontypes::TranslationMatrix{0, -2, 5};
    c.SetTransform(translation);
    auto r = c.RayForPixel(100, 50);
    ASSERT_TRUE(r.origin() == commontypes::Point(0, 2, -5));
    ASSERT_TRUE(r.direction() == commontypes::Vector(sqrt(2) / 2, 0, -sqrt(2) / 2));
}
