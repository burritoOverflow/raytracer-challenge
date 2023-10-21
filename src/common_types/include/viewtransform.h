#ifndef VIEW_TRANSFORM_H
#define VIEW_TRANSFORM_H

#include "point.h"
#include "translationmatrix.h"
#include "vector.h"

namespace commontypes {
class ViewTransform final : public Matrix {
   public:
    ViewTransform() = delete;
    ViewTransform(const commontypes::Point& from,
                  const commontypes::Point& to,
                  const commontypes::Vector& up);
};
}  // namespace commontypes

#endif  // VIEW_TRANSFORM_H
