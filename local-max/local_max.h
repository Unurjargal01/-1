#pragma once

#include <stdexcept>

template <class Iterator>
Iterator LocalMax(Iterator first, Iterator last) {
    Iterator cur = first;
    bool left = true;
    // Empty
    if (first == last) {
        return last;
    } else if (first == --last) {
        return first;
    }
    // Checking first;
    ++first;
    ++last;
    // One element and two elements;
    for (first; first != last; ++first) {
        if (left && *cur < *first) {
            return cur;
        }
        left = (*cur < *first);
        ++cur;
    }
    return last;
}
