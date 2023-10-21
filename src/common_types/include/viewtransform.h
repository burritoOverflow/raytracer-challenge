#ifndef VIEWTRANSFORM_H
#define VIEWTRANSFORM_H

#include "matrix.h"
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

#endif  // VIEWTRANSFORM_H
