#include "matrix.h"
#include "utility.h"

bool operator==(const commontypes::Matrix& m1, const commontypes::Matrix& m2) {
    if (m1.n_rows() != m2.n_rows() || m1.n_columns() != m2.n_columns()) {
        return false;
    }

    for (int row = 0; row < m1.n_rows(); ++row) {
        for (int column = 0; column < m1.n_columns(); ++column) {
            const double m1_element = m1.GetElement(row, column);
            const double m2_element = m2.GetElement(row, column);

            if (!utility::NearEquals(m1_element, m2_element)) {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const commontypes::Matrix& m1, const commontypes::Matrix& m2) {
    return !(m1 == m2);
}
