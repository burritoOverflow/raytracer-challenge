#ifndef PATTERN_H
#define PATTERN_H

#include "color.h"
#include "identitymatrix.h"
#include "matrix.h"
#include "point.h"

// "Pattern" accepts a point in space and returns a color
namespace pattern {
class Pattern {
   public:
    Pattern() : pattern_transform_(commontypes::IdentityMatrix()) {}

    inline void SetPatternTransform(const commontypes::Matrix& transform) {
        pattern_transform_ = transform;
    }

    inline commontypes::Matrix GetPatternTransform() const { return pattern_transform_; }

    // the book's approach is for a shape as a parameter, however the transform for the Shape is
    // used in isolation; the shape itself is irrelevant in this context
    // return the color for the given Pattern, on the provided Shape's transform, at the provided
    // Point in world space; it should respect the transform on both pattern and object
    commontypes::Color PatternAtShape(const commontypes::Matrix& shape_transform,
                                      const commontypes::Point& world_point);

   protected:
    // see discussion on this approach on pg. 133; each derived class implements `PatternAt`
    virtual commontypes::Color PatternAt(const commontypes::Point& point) = 0;

   private:
    commontypes::Matrix pattern_transform_;
};

const commontypes::Color BLACK = {0, 0, 0};
const commontypes::Color WHITE = {1, 1, 1};
}  // namespace pattern

#endif  // PATTERN_H