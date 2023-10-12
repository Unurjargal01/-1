#pragma once
#include <vector>
class Matrix {
public:
    Matrix(const size_t& row, const size_t& col)
        : matrix_(row, std::vector<double>(col)), row_(row), col_(col) {
    }
    Matrix(const size_t& rc) : Matrix(rc, rc) {
    }
    Matrix(const std::vector<std::vector<double>>& v) : row_(v.size()), col_(v[0].size()) {
        for (auto vect : v) {
            matrix_.push_back(std::vector<double>(vect.begin(), vect.end()));
        }
    }

    size_t Rows() const {
        return row_;
    }
    size_t Columns() const {
        return col_;
    }
    double operator()(const size_t& x, const size_t& y) const {
        return matrix_[x][y];
    }
    double& operator()(const size_t& x, const size_t& y) {
        return matrix_[x][y];
    }

    Matrix& operator+=(const Matrix& n) {
        for (size_t i = 0; i < row_; ++i) {
            for (size_t j = 0; j < col_; ++j) {
                matrix_[i][j] += n(i, j);
            }
        }
        return *this;
    }
    Matrix& operator-=(const Matrix& n) {
        for (size_t i = 0; i < row_; ++i) {
            for (size_t j = 0; j < col_; ++j) {
                matrix_[i][j] -= n(i, j);
            }
        }
        return *this;
    }
    Matrix& operator*=(const Matrix& n) {
        std::vector<std::vector<double>> temp(row_, std::vector<double>(n.Columns()));
        for (size_t i = 0; i < row_; ++i) {
            for (size_t j = 0; j < n.Columns(); ++j) {
                for (size_t k = 0; k < col_; ++k) {
                    temp[i][j] += matrix_[i][k] * n(k, j);
                }
            }
        }
        // Without the following code, the matrix printed memory leakage.
        for (auto& row : matrix_) {
            row.clear();
        }
        matrix_.clear();

        matrix_ = temp;
        col_ = n.Columns();
        return *this;
    }
    friend Matrix operator+(const Matrix m, const Matrix& n);
    friend Matrix operator-(const Matrix m, const Matrix& n);
    friend Matrix operator*(const Matrix m, const Matrix& n);

private:
    std::vector<std::vector<double>> matrix_;
    size_t row_, col_;
};

Matrix operator+(Matrix m, const Matrix& n) {
    return m += n;
}
Matrix operator-(Matrix m, const Matrix& n) {
    return m -= n;
}
Matrix operator*(Matrix m, const Matrix& n) {
    return m *= n;
}
Matrix Transpose(const Matrix& m) {
    Matrix temp(m.Columns(), m.Rows());
    for (size_t i = 0; i < m.Rows(); ++i) {
        for (size_t j = 0; j < m.Columns(); ++j) {
            temp(j, i) = m(i, j);
        }
    }
    return temp;
}
Matrix Identity(const size_t& t) {
    Matrix temp(t);
    for (size_t i = 0; i < t; ++i) {
        temp(i, i) = 1.0;
    }
    return temp;
}
