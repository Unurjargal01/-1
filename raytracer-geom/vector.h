#pragma once

#include <array>
#include <cstddef>
#include <cmath>

class Vector {
public:
    // why need to initiate empty constructor;
    
    Vector(double x, double y, double z) {
        data_[0] = x;
        data_[1] = y;
        data_[2] = z;
    }
    Vector() {};
    double& operator[](size_t ind) {
        return data_[ind];
    };
    double operator[](size_t ind) const {
        return data_[ind];
    };

    double Length() const {
        double ans = 0;
        for (size_t i = 0; i < 3; ++i) {
            ans += data_[i] * data_[i];
        }
        return std::sqrt(ans);
    }

    void Normalize() {
        double len = this->Length();
        for (auto& i : data_) {
            i /= len;
        }
    };
    // Extra operations
    Vector& operator+=(const Vector& rhs) {
        data_[0] += rhs[0];
        data_[1] += rhs[1];
        data_[2] += rhs[2];
        return *this;
    }

    Vector& operator*=(const double& coef) {
        data_[0] *= coef;
        data_[1] *= coef;
        data_[2] *= coef;
        return *this;
    }
    friend const Vector operator+(const Vector& lhs, const Vector& rhs) {
        return Vector{lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]};
    }
    friend const Vector operator-(const Vector& lhs, const Vector& rhs) {
        return Vector{lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]};
    }

    friend const Vector operator*(const Vector& lhs, const double& coef) {
        return Vector{lhs[0] * coef, lhs[1] * coef, lhs[2] * coef};
    }
    friend const Vector operator*(const double& coef, const Vector& lhs) {
        return Vector{lhs[0] * coef, lhs[1] * coef, lhs[2] * coef};
    }
    Vector& operator/(const double nom) {
        data_[0] /= nom;
        data_[1] /= nom;
        data_[2] /= nom;
        return *this;
    }
    // Just in case: rule of 3

private:
    std::array<double, 3> data_ = {};
};

double DotProduct(const Vector& a, const Vector& b) {
    double ans = 0;
    for (int i = 0; i < 3; ++i) {
        ans += a[i] * b[i];
    }
    return ans;
};
Vector CrossProduct(const Vector& a, const Vector& b) {
    double x, y, z;
    x = a[1] * b[2] - a[2] * b[1];
    y = -(a[0] * b[2] - a[2] * b[0]);
    z = a[0] * b[1] - a[1] * b[0];
    return Vector{x, y, z};
}
double Length(const Vector& v) {
    return std::sqrt(DotProduct(v, v));
};
