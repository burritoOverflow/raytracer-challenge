#ifndef SCALINGMATRIX_H
#define SCALINGMATRIX_H

#include "matrix.h"

namespace commontypes {
class ScalingMatrix final : public Matrix {
   public:
    ScalingMatrix(const double x_scaling_value,
                  const double y_scaling_value,
                  const double z_scaling_value)
        : Matrix(4, 4) {
        (*this)(0, 0) = x_scaling_value;
        (*this)(1, 1) = y_scaling_value;
        (*this)(2, 2) = z_scaling_value;
        (*this)(3, 3) = 1.0;
    }
};
}  // namespace commontypes

#endif  // SCALINGMATRIX_H
