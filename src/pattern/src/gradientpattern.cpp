#include "gradientpattern.h"
#include <cmath>
#include "color.h"

commontypes::Color pattern::GradientPattern::PatternAt(const commontypes::Point& point) {
    // uses a blending function (see pg. 135), interpolates between the two values
    const commontypes::Color distance = commontypes::Color{color_b_ - color_a_};
    const double fraction = point.x() - floor(point.x());
    return commontypes::Color{color_a_ + distance * fraction};
}