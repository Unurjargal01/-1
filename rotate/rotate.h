#pragma once

#include <stdexcept>
#include <vector>
#include <cstddef>
// https://cplusplus.com/reference/algorithm/rotate/
void Rotate(std::vector<int>* data, size_t shift) {
    if (shift == 0) {
        return;
    }
    shift %= data->size();
    size_t first = 0, middle = shift, last = data->size();
    size_t next = middle;
    while (first != next) {
        std::swap(data->at(first++), data->at(next++));
        if (next == last) {
            next = middle;
        } else if (first == middle) {
            middle = next;
        }
    }
}
