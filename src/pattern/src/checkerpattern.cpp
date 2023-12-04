#include "checkerpattern.h"
#include <math.h>

commontypes::Color pattern::CheckerPattern::PatternAt(const commontypes::Point& point) {
    // see pg. 137
    const double sum_floors = floor(point.x()) + floor(point.y()) + floor(point.z());
    if (fmod(sum_floors, 2) == 0) {
        return color_a_;
    }
    return color_b_;
}