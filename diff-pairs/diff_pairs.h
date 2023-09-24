#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <cmath>

int64_t CountPairs(const std::vector<int>& data, int x) {
    std::map<int, int> counter;
    int64_t cnt = 0;
    for (const int& v : data) {
        int64_t dif = x - v;
        if (std::abs(dif) <= std::pow(2, 31) - 1) {
            if (counter.find(x - v) != counter.end()) {
                cnt += counter[x - v];
            }
            if (counter.insert(std::pair<int, int>(v, 1)).second == 0) {
                counter[v] += 1;
            }
        }
    }
    return cnt;
}
