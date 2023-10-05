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

double DetNorm(const Point& a, const Point& b) {
    return (static_cast<double>(a.x) * b.y - static_cast<double>(a.y) * b.x);
}
bool Inside(const Point& a, const Point& b, const Point& c, const Point& pt) {
    Point u{b.x - a.x, b.y - a.y}, v{c.x - a.x, c.y - a.y}, dif{pt.x - a.x, pt.y - a.y};
    double n = (DetNorm(dif, u) - DetNorm(a, u)) / DetNorm(v, u);
    double m = -(DetNorm(dif, v) - DetNorm(a, v)) / DetNorm(v, u);

    if (n >= 0 && m >= 0 && (n + m <= 1)) {
        return true;
    }
    return false;
}
bool IsPointInTriangle(const Triangle& t, const Point& pt) {
    Point a1 = t.a, b1 = t.b, c1 = t.c;
    return Inside(a1, b1, c1, pt);
}
