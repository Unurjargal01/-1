#pragma once

#include <utility>
#include <vector>
#include <stdexcept>
#include <cmath>

std::vector<std::pair<int64_t, int>> Factorize(int64_t x) {
    std::vector<std::pair<int64_t, int>> factor;
    if (x == 2 || x == 3) {
        factor.emplace_back(x, 1);
        return factor;
    }
    for (int64_t i = 2; i * i <= x; ++i) {
        int64_t cnt = 0;
        while (x % i == 0) {
            x /= i;
            cnt += 1;
        }
        if (cnt) {
            factor.emplace_back(i, cnt);
        }
    }
    if (x > 1) {
        factor.emplace_back(x, 1);
    }
    return factor;
}