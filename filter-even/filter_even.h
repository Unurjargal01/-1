#pragma once

#include <vector>
#include <stdexcept>
// Erase-remove idiom

void FilterEven(std::vector<int>* data) {
    size_t it = 0, cur = 0;
    while (cur < data->size()) {
        if (data->at(cur) % 2 == 0) {
            std::swap(data->at(it), data->at(cur));
            ++it;
        }
        ++cur;
    }
    data->erase(data->begin() + it, data->end());
}
