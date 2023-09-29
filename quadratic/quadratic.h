#pragma once

#include <stdexcept>
#include <cmath>
enum class RootCount { kZero, kOne, kTwo, kInf };

struct Roots {
    RootCount count;
    double first;
    double second;
};

Roots SolveQuadratic(int a, int b, int c) {
    Roots ans;
    if (a == 0) {
        if (b == 0 && c == 0) {
            ans.count = RootCount::kInf;
        } else if (b == 0 && c != 0) {
            ans.count = RootCount::kZero;
        } else {
            ans.count = RootCount::kOne;
            ans.first = -static_cast<double>(c) / b;
        }
    } else {
        double b1 = static_cast<double>(b) / a, c1 = static_cast<double>(c) / a;
        double diap = b1 * b1 - 4 * c1;
        if (diap == 0) {
            ans.count = RootCount::kOne;
            ans.first = -b1 / 2;
        } else if (diap > 0) {
            double min = (-b1 - std::sqrt(diap)) / 2, max = (-b1 + std::sqrt(diap)) / 2;
            if (min > max) {
                std::swap(min, max);
            }
            ans.count = RootCount::kTwo;
            ans.first = min;
            ans.second = max;
        } else {
            ans.count = RootCount::kZero;
        }
    }
    return ans;
}
