#ifndef TRANSLATIONMATRIX_H
#define TRANSLATIONMATRIX_H

#include "identitymatrix.h"

namespace commontypes {

// start with an identity matrix and change the desired x,y,z values to
// at the t03, t13, t23 elements, respectively
class TranslationMatrix final : public IdentityMatrix {
   public:
    TranslationMatrix(const double x_translation,
                      const double y_translation,
                      const double z_translation) {
        const size_t identity_col_idx = 3;
        (*this)(0, identity_col_idx) = x_translation;
        (*this)(1, identity_col_idx) = y_translation;
        (*this)(2, identity_col_idx) = z_translation;
    }
};
}  // namespace commontypes

#endif  // TRANSLATIONMATRIX_H
