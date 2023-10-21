#include "camera.h"
#include <gtest/gtest.h>

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
