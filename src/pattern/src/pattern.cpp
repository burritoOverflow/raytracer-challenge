#include "pattern.h"

commontypes::Color pattern::Pattern::PatternAtShape(const commontypes::Matrix& shape_transform,
                                                    const commontypes::Point& world_point) const {
    // this is the implementation of the initial approach outlined on pg. 132, and
    // revised by the approach on pg. 133

    // world-space-point * inverse of object's Transform to convert point to object space
    const commontypes::Point object_point =
        commontypes::Point{shape_transform.Inverse() * world_point};

    // object-space-point * inverse of pattern's transformation matrix to convert point to pattern
    // space
    const commontypes::Point pattern_point =
        commontypes::Point(pattern_transform_.Inverse() * object_point);

    // delegate this result to each individual Pattern's implementation
    return PatternAt(pattern_point);
}
