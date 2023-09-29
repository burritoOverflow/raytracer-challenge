#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

using matrixtype = std::vector<std::vector<double>>;

namespace commontypes {
class Matrix {
   public:
    Matrix(const size_t n_rows, const size_t n_columns) : n_rows_(n_rows), n_columns_(n_columns) {
        const std::vector<double> col_vec(n_columns_);
        matrix_.resize(n_rows_, col_vec);

        for (int row = 0; row < n_rows_; ++row) {
            for (int column = 0; column < n_columns_; ++column) {
                matrix_.at(row).at(column) = 0.0;
            }
        }
    }

    Matrix(matrixtype matrix) : matrix_(std::move(matrix)) {
        n_rows_ = matrix_.size();
        n_columns_ = matrix_[0].size();
    }

    ~Matrix() = default;

    void SetElement(const size_t row_idx, const size_t column_idx, const double value) {
        assert(row_idx >= 0 && row_idx < n_rows_);
        assert(column_idx >= 0 && column_idx < n_columns_);
        matrix_.at(row_idx).at(column_idx) = value;
    }

    double GetElement(const size_t row_idx, const size_t column_idx) const {
        assert(row_idx >= 0 && row_idx < n_rows_);
        assert(column_idx >= 0 && column_idx < n_columns_);
        return matrix_.at(row_idx).at(column_idx);
    }

    inline double& operator()(const size_t row_idx, const size_t column_idx) {
        assert(row_idx >= 0 && row_idx < n_rows_);
        assert(column_idx >= 0 && column_idx < n_columns_);
        return matrix_.at(row_idx).at(column_idx);
    }

   private:
    size_t n_rows_;
    size_t n_columns_;
    matrixtype matrix_;
};
}  // namespace commontypes

#endif  // MATRIX_H