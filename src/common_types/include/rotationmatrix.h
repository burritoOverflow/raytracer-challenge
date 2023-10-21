#ifndef ROTATIONMATRIX_H
#define ROTATIONMATRIX_H

#include <cmath>
#include "matrix.h"

namespace commontypes {
class RotationMatrixX final : public Matrix {
   public:
    RotationMatrixX(const double radians) : Matrix(4, 4) {
        (*this)(0, 0) = 1;
        (*this)(1, 1) = cos(radians);
        (*this)(1, 2) = -sin(radians);
        (*this)(2, 1) = sin(radians);
        (*this)(2, 2) = cos(radians);
        (*this)(3, 3) = 1;
    }
};

class RotationMatrixY final : public Matrix {
   public:
    RotationMatrixY(const double radians) : Matrix(4, 4) {
        (*this)(0, 0) = cos(radians);
        (*this)(0, 2) = sin(radians);
        (*this)(1, 1) = 1;
        (*this)(2, 0) = -sin(radians);
        (*this)(2, 2) = cos(radians);
        (*this)(3, 3) = 1;
    }
};

class RotationMatrixZ final : public Matrix {
   public:
    RotationMatrixZ(const double radians) : Matrix(4, 4) {
        (*this)(0, 0) = cos(radians);
        (*this)(0, 1) = -sin(radians);
        (*this)(1, 0) = sin(radians);
        (*this)(1, 1) = cos(radians);
        (*this)(2, 2) = 1;
        (*this)(3, 3) = 1;
    }
};
}  // namespace commontypes

#endif  // ROTATIONMATRIX_H
