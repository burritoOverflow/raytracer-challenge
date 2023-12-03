#include "ringpattern.h"
#include <math.h>
#include "color.h"

// tests the distance of the point in both X and Z (see pg. 135)
commontypes::Color pattern::RingPattern::PatternAt(const commontypes::Point& point) {
    const double x_sq = pow(point.x(), 2);
    const double z_sq = pow(point.z(), 2);
    const double sum_squared = sqrt(x_sq + z_sq);

    if (fmod(floor(sum_squared), 2) == 0) {
        return color_a_;
    }
    return color_b_;
}