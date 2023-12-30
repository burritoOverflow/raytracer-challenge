#include "stripepattern.h"
#include <cmath>

commontypes::Color pattern::StripePattern::PatternAt(const commontypes::Point& point) const {
    // as x coord changes, the pattern alternates between the two colors
    // i.e x coord between 0-1 return color_a, other wise color_b
    if (fmod(floor(point.x()), 2) == 0) {
        return color_a_;
    }
    return color_b_;
}