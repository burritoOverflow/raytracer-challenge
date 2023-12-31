#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include <cstddef>
#include <vector>
#include "tuple.h"

using matrixtype = std::vector<std::vector<double>>;

namespace commontypes {
class Matrix {
   public:
    Matrix() = delete;

    // default initialize to 0.0
    explicit Matrix(const size_t n_rows, const size_t n_columns)
        : n_rows_(n_rows), n_columns_(n_columns) {
        const std::vector<double> col_vec(n_columns_);
        matrix_.resize(n_rows_, col_vec);

        for (int row = 0; row < n_rows_; ++row) {
            for (int column = 0; column < n_columns_; ++column) {
                matrix_.at(row).at(column) = 0.0;
            }
        }
    }

    explicit Matrix(matrixtype matrix)
        : matrix_(std::move(matrix)), n_rows_(matrix.size()), n_columns_(matrix[0].size()) {}

    ~Matrix() = default;

    inline size_t n_rows() const { return n_rows_; }

    inline size_t n_columns() const { return n_columns_; }

    inline matrixtype matrix() const { return matrix_; }

    void SetElement(const size_t row_idx, const size_t column_idx, const double value) {
        AssertBounds(row_idx, column_idx);
        matrix_.at(row_idx).at(column_idx) = value;
    }

    double GetElement(const size_t row_idx, const size_t column_idx) const {
        AssertBounds(row_idx, column_idx);
        return matrix_.at(row_idx).at(column_idx);
    }

    Matrix Transpose() const;

    double Determinant() const;

    // Returns a new submatrix of this matrix with the row at `row_idx` and the column at
    // `column_idx` removed
    Matrix Submatrix(size_t row_idx, size_t column_idx) const;

    // Minor at row_idx, column_idx is the determinant of the submatrix at row_idx, column_idx
    double Minor(size_t row_idx, size_t column_idx) const;

    double Cofactor(size_t row_idx, size_t column_idx) const;

    bool IsInvertible() const;

    // return the inverse of the current matrix
    Matrix Inverse() const;

    inline double& operator()(const size_t row_idx, const size_t column_idx) {
        AssertBounds(row_idx, column_idx);
        return matrix_.at(row_idx).at(column_idx);
    }

   protected:
    size_t n_rows_;
    size_t n_columns_;
    matrixtype matrix_;

   private:
    void AssertBounds(const size_t row_idx, const size_t column_idx) const {
        assert(row_idx >= 0 && row_idx < n_rows_);
        assert(column_idx >= 0 && column_idx < n_columns_);
    }
};
}  // namespace commontypes

commontypes::Matrix operator*(const commontypes::Matrix& m1, const commontypes::Matrix& m2);
commontypes::Tuple operator*(const commontypes::Matrix& m, const commontypes::Tuple& t);
commontypes::Tuple operator*(const commontypes::Tuple& t, const commontypes::Matrix& m);
bool operator==(const commontypes::Matrix& m1, const commontypes::Matrix& m2);
bool operator!=(const commontypes::Matrix& m1, const commontypes::Matrix& m2);

#endif  // MATRIX_H
