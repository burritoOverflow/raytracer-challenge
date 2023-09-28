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

TEST(CanvasTests, TestWritePPMHeader) {
    Canvas canvas{5, 3};
    std::string canvas_str = canvas.WritePPM();
    std::istringstream canvas_istream(canvas_str);

    std::string ppm_str;
    std::getline(canvas_istream, ppm_str);
    EXPECT_EQ(ppm_str, "P3");

    std::string width_height_str;
    std::getline(canvas_istream, width_height_str);
    EXPECT_EQ(width_height_str, "5 3");

    std::string max_color_value_str;
    std::getline(canvas_istream, max_color_value_str);
    EXPECT_EQ(max_color_value_str, "255");
}

TEST(CanvasTests, TestWritePixelDataToPPM) {
    Canvas canvas{5, 3};
    commontypes::Color c1{1.5, 0, 0};
    commontypes::Color c2{0, 0.5, 0};
    commontypes::Color c3{-0.5, 0, 1};

    canvas.WritePixel(0, 0, c1);
    canvas.WritePixel(2, 1, c2);
    canvas.WritePixel(4, 2, c3);

    std::istringstream canvas_istream(canvas.WritePPM());

    // consume the header, as above
    std::string ppm_str;
    std::getline(canvas_istream, ppm_str);
    EXPECT_EQ(ppm_str, "P3");

    std::string width_height_str;
    std::getline(canvas_istream, width_height_str);
    EXPECT_EQ(width_height_str, "5 3");

    std::string max_color_value_str;
    std::getline(canvas_istream, max_color_value_str);
    EXPECT_EQ(max_color_value_str, "255");

    std::string row1, row2, row3;
    std::getline(canvas_istream, row1);
    std::getline(canvas_istream, row2);
    std::getline(canvas_istream, row3);
    EXPECT_EQ("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0", row1);
    EXPECT_EQ("0 0 0 0 0 0 0 128 0 0 0 0 0 0 0", row2);
    EXPECT_EQ("0 0 0 0 0 0 0 0 0 0 0 0 0 0 255", row3);
}

TEST(CanvasTests, TestSplittingLongLinesPPM) {
    const size_t width = 10;
    const size_t height = 2;
    Canvas canvas{width, height};
    commontypes::Color color{1.0, 0.8, 0.6};
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            canvas.WritePixel(x, y, color);
        }
    }

    std::istringstream canvas_istream(canvas.WritePPM());

    std::string ppm_str;
    std::getline(canvas_istream, ppm_str);
    EXPECT_EQ(ppm_str, "P3");

    std::string width_height_str;
    std::getline(canvas_istream, width_height_str);
    EXPECT_EQ(width_height_str, "10 2");

    std::string max_color_value_str;
    std::getline(canvas_istream, max_color_value_str);
    EXPECT_EQ(max_color_value_str, "255");

    std::string row1, row2, row3, row4;
    std::getline(canvas_istream, row1);
    std::getline(canvas_istream, row2);
    std::getline(canvas_istream, row3);
    std::getline(canvas_istream, row4);
    EXPECT_EQ("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204", row1);
    EXPECT_EQ("153 255 204 153 255 204 153 255 204 153 255 204 153", row2);
    EXPECT_EQ("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204", row3);
    EXPECT_EQ("153 255 204 153 255 204 153 255 204 153 255 204 153", row4);
}

TEST(CanvasTests, TestCanvasPPMFilesAreNewlineTerminated) {
    Canvas canvas{5, 3};
    const std::string ppm_str = canvas.WritePPM();
    // verify the final line has a newline
    EXPECT_EQ('\n', ppm_str.at(ppm_str.size() - 1));
}