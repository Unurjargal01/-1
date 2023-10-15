#pragma once

#include <stdexcept>
#include <utility>

template <class Iterator, class Predicate>
Iterator Partition(Iterator first, Iterator last, Predicate pred) {
    Iterator group = first;
    for (; first != last; ++first) {
        if (pred(*first)) {
            std::swap(*group, *first);
            ++group;
        }
    }
    return group;
}
