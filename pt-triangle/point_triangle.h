#pragma once

#include <stdexcept>
#include <cmath>

struct Point {
    int x, y;
};

struct Triangle {
    Point a, b, c;
};

bool Onborder(const Point& a, const Point& b, const Point& pt) {
    if (((a.x == pt.x) && (a.y == pt.y)) || ((b.x == pt.x) && (b.y == pt.y))) {
        return true;
    } else {
        double ct = static_cast<double>(b.x - a.x) / (pt.x - a.x);
        double ct1 = static_cast<double>(b.y - a.y) / (pt.y - a.y);
        if (ct == ct1) {
            return true;
        }
    }
    return false;
}
double DetNorm(const Point& a, const Point& b) {
    return static_cast<double>(a.x * b.y - a.y * b.x) / std::pow(10., 5);
}
bool Inside(const Point& a, const Point& b, const Point& c, const Point& pt) {
    Point u{b.x - a.x, b.y - a.y}, v{c.x - a.x, c.y - a.y}, dif{pt.x - a.x, pt.y - a.y};
    double n = (DetNorm(dif, u) - DetNorm(a, u)) / DetNorm(v, u);
    double m = -(DetNorm(dif, v) - DetNorm(a, v)) / DetNorm(v, u);

    if (n > 0 && m > 0 && (n + m < 1)) {
        return true;
    }
    return false;
}
bool IsPointInTriangle(const Triangle& t, const Point& pt) {
    Point a1 = t.a, b1 = t.b, c1 = t.c;
    if (Onborder(a1, b1, pt) || Onborder(b1, c1, pt) || Onborder(c1, a1, pt)) {
        return true;
    } else if (Inside(a1, b1, c1, pt)) {
        return true;
    }
    return false;
}
