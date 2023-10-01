#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

#include <map>
#include <cmath>

int64_t CountPairs(const std::vector<int>& data, int x) {
    std::map<int64_t, int> stor;
    int64_t y = x, diff;
    int64_t cnt = 0;
    for (const auto& i : data) {
        diff = y - i;
        if (stor.find(diff) != stor.end()) {
            cnt += stor[diff];
        }
        if (!stor.insert(std::pair<int, int>(i, 1)).second) {
            stor[i] += 1;
        }
    }
    return cnt;
}
