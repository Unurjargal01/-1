#pragma once

#include <vector>
#include <stdexcept>

std::vector<int> Range(int from, int to, int step = 1) {
    std::vector<int> v;
    
    if (from < to && step > 0) {
        for (int i = from; i < to; i += step) {
            v.push_back(i);
        }
    } else if (from > to && step < 0) {
        for (int i = from; i > to; i += step) {
            v.push_back(i);
        }
    }
    return v;
}
