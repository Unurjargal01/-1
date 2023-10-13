#pragma once

#include <stdexcept>

template <class Iterator, class T>
Iterator FindLast(Iterator first, Iterator last, const T& value) {
    Iterator ans = last;
    // Empty and One element
    if (first == last) {
        return last;
    } else if (first == --last) {
        return (*first == value) ? first : ans;
    }
    for (Iterator it = last; it != first; --it) {
        if (*it == value) {
            ans = it;
            break;
        }
    }
    return ans;
}