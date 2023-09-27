#include "canvas.h"
#include <gtest/gtest.h>

TEST(CanvasTests, TestCreateCanvas) {
    Canvas canvas{10, 20};
    commontypes::Color expected{};

    for (size_t i = 0; i < canvas.width(); i++) {
        for (size_t j = 0; j < canvas.height(); j++) {
            ASSERT_TRUE(canvas.GetPixel(i, j) == expected);
        }
    }
}

TEST(CanvasTests, TestWritePixelToCanvas) {
    Canvas canvas{10, 20};
    commontypes::Color red{1, 0, 0};
    const size_t x = 2;
    const size_t y = 3;
    canvas.WritePixel(x, y, red);
    ASSERT_TRUE(canvas.GetPixel(x, y) == red);
}
