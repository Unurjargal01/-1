#pragma once

#include <stdexcept>

template <class Iterator, class T>
Iterator FindLast(Iterator first, Iterator last, const T& value) {
    Iterator ans = last;
    for (Iterator it = first; it != last; ++it) {
        if (*it == value) {
            ans = it;
        }
    }
    return ans;
}
