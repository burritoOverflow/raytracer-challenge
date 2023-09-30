#include "matrix.h"
#include <stdexcept>
#include "tuple.h"
#include "utility.h"

commontypes::Matrix commontypes::Matrix::Transpose() const {
    commontypes::Matrix result{n_columns_, n_rows_};

    for (size_t row = 0; row < n_rows_; ++row) {
        for (size_t column = 0; column < n_columns_; ++column) {
            result(row, column) = GetElement(column, row);
        }
    }

    return result;
}

double commontypes::Matrix::Determinant() const {
    // for 2x2 Matrix: Det == ad - bc
    if (n_rows_ == 2 && n_columns_ == 2) {
        return GetElement(0, 0) * GetElement(1, 1) - GetElement(0, 1) * GetElement(1, 0);
    }

    double determinant{0.0};
    const size_t row_idx{0};
    // for each element multiply the element by its cofactor; add these products together
    for (size_t column = 0; column < n_columns_; ++column) {
        determinant += GetElement(row_idx, column) * Cofactor(row_idx, column);
    }

    return determinant;
}

commontypes::Matrix commontypes::Matrix::Submatrix(const size_t row_idx,
                                                   const size_t column_idx) const {
    assert(row_idx >= 0 && row_idx < n_rows_);
    assert(column_idx >= 0 && column_idx < n_columns_);

    commontypes::Matrix submatrix{n_rows_ - 1, n_columns_ - 1};

    for (size_t row = 0; row < n_rows_; ++row) {
        if (row == row_idx)
            continue;

        for (size_t column = 0; column < n_columns_; ++column) {
            if (column == column_idx)
                continue;

            // true when the previous iteration == the corresponding index
            const size_t r = row >= row_idx ? row - 1 : row;
            const size_t c = column >= column_idx ? column - 1 : column;
            submatrix(r, c) = GetElement(row, column);
        }
    }

    return submatrix;
}

double commontypes::Matrix::Minor(const size_t row_idx, const size_t column_idx) const {
    return Submatrix(row_idx, column_idx).Determinant();
}

double commontypes::Matrix::Cofactor(const size_t row_idx, const size_t column_idx) const {
    const double minor = Minor(row_idx, column_idx);
    if (row_idx + column_idx % 2 == 0) {
        return minor;
    }
    return -minor;
}

static commontypes::Tuple MultiplyMatrixTuple(const commontypes::Matrix& m,
                                              const commontypes::Tuple& t) {
    assert(m.n_rows() == 4 && m.n_columns() == 4);
    commontypes::Tuple result{};

    for (size_t row = 0; row < m.n_rows(); ++row) {
        double row_result = 0.0;
        for (size_t i = 0; i < m.n_columns(); ++i) {
            row_result += m.GetElement(row, i) * t.e_[i];
        }
        result.e_[row] = row_result;
    }

    return result;
}

commontypes::Matrix operator*(const commontypes::Matrix& m1, const commontypes::Matrix& m2) {
    assert(m1.n_columns() == m2.n_rows());
    commontypes::Matrix result(m1.n_rows(), m2.n_columns());

    for (int row = 0; row < m1.n_rows(); ++row) {
        for (int column = 0; column < m2.n_columns(); ++column) {
            double accum{0.0};
            for (size_t idx = 0; idx < m1.n_columns(); ++idx) {
                accum += m1.GetElement(row, idx) * m2.GetElement(idx, column);
            }
            result(row, column) = accum;
        }
    }

    return result;
}

// expected the only use case is a 4x4 Matrix, implicity in the book
commontypes::Tuple operator*(const commontypes::Matrix& m, const commontypes::Tuple& t) {
    return MultiplyMatrixTuple(m, t);
}

commontypes::Tuple operator*(const commontypes::Tuple& t, const commontypes::Matrix& m) {
    return MultiplyMatrixTuple(m, t);
}

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
