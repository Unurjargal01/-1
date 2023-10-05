#pragma once

#include <stdexcept>
#include <cmath>
#include <algorithm>

struct Point {
    int x, y;
};

struct Triangle {
    Point a, b, c;
};
// Explicit calculation of barycenter coordinates;
bool IsPointInTriangle(const Triangle& t, const Point& pt) {
    Point u{t.b.x - t.a.x, t.b.y - t.a.y}, v{t.c.x - t.a.x, t.c.y - t.a.y},
        diff{pt.x - t.a.x, pt.y - t.a.y};
    double det = static_cast<double>(u.x) * v.y - static_cast<double>(v.x) * u.y;
    double lambda1 = (static_cast<double>(v.y) * diff.x - static_cast<double>(v.x) * diff.y) / det;
    double lambda2 = (-static_cast<double>(u.y) * diff.x + static_cast<double>(u.x) * diff.y) / det;
    double lambda3 = 1 - lambda1 - lambda2;
    return (lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0) ? true : false;
}
