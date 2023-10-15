#pragma once

#include <stdexcept>

template <class Iterator>
Iterator LocalMax(Iterator first, Iterator last) {
    Iterator cur = first;
    bool prev = true;
    if (first == last) {
        return last;
    } else if (++first == last) {
        return --first;
    }
    // Careful with equality
    while (first != last) {
        if (prev && (*first < *cur)) {
            return cur;
        }
        prev = (*cur < *first);
        ++cur;
        ++first;
    }

    return last;
}
