#pragma once

#include <vector.h>

class Triangle {
public:
    Triangle(const Vector& a, const Vector& b, const Vector& c);

    const Vector& operator[](size_t ind) const;
    double Area() const;
};
