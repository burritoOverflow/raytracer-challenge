#include "matrix.h"
#include <stdexcept>
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
    if ((row_idx + column_idx) % 2 == 0) {
        return minor;
    }

    return -minor;
}

bool commontypes::Matrix::IsInvertible() const {
    const double determinant = Determinant();
    // invertible as long as the determinant is not 0
    return !utility::NearEquals(0.0, determinant);
}

commontypes::Matrix commontypes::Matrix::Inverse() const {
    if (!IsInvertible()) {
        throw std::invalid_argument("Matrix is not invertible");
    }

    Matrix inverse_matrix{n_rows_, n_columns_};
    const double determinant = Determinant();
    for (int row_idx = 0; row_idx < n_rows_; ++row_idx) {
        for (int column_idx = 0; column_idx < n_columns_; ++column_idx) {
            const double cofactor = Cofactor(row_idx, column_idx);
            inverse_matrix(column_idx, row_idx) = cofactor / determinant;
        }
    }

    return inverse_matrix;
}

static commontypes::Tuple MultiplyMatrixTuple(const commontypes::Matrix& m,
                                              const commontypes::Tuple& t) {
    assert(m.n_rows() == 4 && m.n_columns() == 4);
    commontypes::Tuple result{};

    // treat Tuple as a single column matrix
    for (size_t row = 0; row < m.n_rows(); ++row) {
        // each element is the sum of the products of each tuple element and the matrix's
        // elements in that row (see page 31)--i.e dot product of each row and the other tuple
        double row_result = 0.0;
        for (size_t i = 0; i < m.n_columns(); ++i) {
            row_result += m.GetElement(row, i) * t.e_[i];
        }

        result.e_[row] = row_result;
    }

    return result;
}

// The number of columns in the first matrix must be equal to the number of rows in the second
// matrix; Resulting matrix has the number of rows from the first and the number of columns
// from the second
commontypes::Matrix operator*(const commontypes::Matrix& m1, const commontypes::Matrix& m2) {
    // necessary pre-condition
    assert(m1.n_columns() == m2.n_rows());

    // construct matrix product based on the discussion above
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

//  the only expected use case is a 4x4 Matrix, implicitly in this book
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
