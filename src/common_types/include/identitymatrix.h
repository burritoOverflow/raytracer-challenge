#ifndef IDENTITYMATRIX_H
#define IDENTITYMATRIX_H

#include "matrix.h"

namespace commontypes {
class IdentityMatrix : public Matrix {
   public:
    IdentityMatrix() : Matrix(4, 4) {
        for (size_t i = 0; i < 4; ++i) {
            (*this)(i, i) = 1;
        }
    }
};
}  // namespace commontypes

#endif  // IDENTITYMATRIX_H
