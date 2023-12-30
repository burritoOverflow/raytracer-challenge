#ifndef SHEARINGMATRIX_H
#define SHEARINGMATRIX_H

#include "identitymatrix.h"

namespace commontypes {
class ShearingMatrix final : public IdentityMatrix {
   public:
    explicit ShearingMatrix(double x_y, double x_z, double y_x, double y_z, double z_x, double z_y)
        : IdentityMatrix() {
        (*this)(0, 1) = x_y;
        (*this)(0, 2) = x_z;
        (*this)(1, 0) = y_x;
        (*this)(1, 2) = y_z;
        (*this)(2, 0) = z_x;
        (*this)(2, 1) = z_y;
    }
};
}  // namespace commontypes

#endif  // SHEARINGMATRIX_H
