#ifndef VIEW_TRANSFORM_H
#define VIEW_TRANSFORM_H

#include "point.h"
#include "translationmatrix.h"
#include "vector.h"

namespace commontypes {
class ViewTransform final : public Matrix {
   public:
    ViewTransform() = delete;
    ViewTransform(commontypes::Point& from, commontypes::Point& to, commontypes::Vector& up);
};
}  // namespace commontypes

#endif  // VIEW_TRANSFORM_H
