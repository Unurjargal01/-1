#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <cmath>

int64_t CountPairs(const std::vector<int>& data, int x) {
    std::map<int, int> counter;
    int cnt = 0;
    int int_max = std::pow(2, 31) - 1;
    for (const int& v : data) {
        if (((x > 0) && ((x - int_max) > v)) || ((x < 0) && ((v - x) > int_max))) {
            continue;
        }
        int dif = x - v;

        if (counter.find(dif) != counter.end()) {
            cnt += counter[dif];
        }
        if (counter.insert(std::pair<int, int>(v, 1)).second == 0) {
            counter[v] += 1;
        }
    }
    return cnt;
}
