
#ifndef CHECKER_PATTERN_H
#define CHECKER_PATTERN_H

#include "color.h"
#include "pattern.h"

namespace pattern {
class CheckerPattern : public Pattern {
   public:
    CheckerPattern()
        : Pattern(),
          color_a_(commontypes::Color::MakeWhite()),
          color_b_(commontypes::Color::MakeBlack()) {}

    CheckerPattern(const commontypes::Color& color_a, const commontypes::Color& color_b)
        : Pattern(), color_a_(color_a), color_b_(color_b) {}

    commontypes::Color PatternAt(const commontypes::Point& point) const override;

    commontypes::Color ColorA() const { return color_a_; }

    commontypes::Color ColorB() const { return color_b_; }

   private:
    commontypes::Color color_a_;
    commontypes::Color color_b_;
};
}  // namespace pattern

#endif  // CHECKER_PATTERN_H