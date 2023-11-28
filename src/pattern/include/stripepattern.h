#ifndef STRIPE_PATTERN_H
#define STRIPE_PATTERN_H
#include "color.h"
#include "pattern.h"

namespace pattern {
class StripePattern : public Pattern {
   public:
    // set black, white as the default Colors
    StripePattern() : Pattern(), color_a_({0, 0, 0}), color_b_({1, 1, 1}) {}

    StripePattern(const commontypes::Color& color_a, const commontypes::Color& color_b)
        : Pattern(), color_a_(color_a), color_b_(color_b) {}

    commontypes::Color PatternAt(const commontypes::Point& point) override;

    commontypes::Color ColorA() const { return color_a_; }

    commontypes::Color ColorB() const { return color_b_; }

   private:
    // the two alternating colors that comprise the pattern
    commontypes::Color color_a_;
    commontypes::Color color_b_;
};
}  // namespace pattern

#endif  // STRIPE_PATTERN_H