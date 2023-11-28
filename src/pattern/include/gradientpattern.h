#ifndef GRADIENT_PATTERN_H
#define GRADIENT_PATTERN_H
#include "color.h"
#include "pattern.h"

namespace pattern {
class GradientPattern : public Pattern {
   public:
    // as implemented in StripePattern
    GradientPattern() : Pattern(), color_a_(WHITE), color_b_(BLACK) {}

    GradientPattern(const commontypes::Color& color_a, const commontypes::Color& color_b)
        : Pattern(), color_a_(color_a), color_b_(color_b) {}

    commontypes::Color PatternAt(const commontypes::Point& point) override;

    commontypes::Color ColorA() const { return color_a_; }

    commontypes::Color ColorB() const { return color_b_; }

   private:
    commontypes::Color color_a_;
    commontypes::Color color_b_;
};
}  // namespace pattern

#endif  // GRADIENT_PATTERN_H