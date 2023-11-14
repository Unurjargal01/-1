#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <memory>
template <typename Iterator, typename Predicate, typename Functor>
void TransformIf(Iterator begin, Iterator end, Predicate p, Functor f) {
    using T = typename std::iterator_traits<Iterator>::value_type;
    std::vector<Iterator> log_it;
    // bool copy_fail = false;
    for (Iterator it = begin; it != end; ++it) {
        try {
            if (p(*it)) {
                log_it.push_back(it);
            }
        } catch (...) {
            throw;
        }

        try {
            if (p(*it)) {
                T a = *it;
                f(a);
            }
        } catch (const std::overflow_error&) {
            continue;
        } catch (const std::logic_error&) {
            continue;
        } catch (...) {
            throw;
        }
    }
    for (auto it : log_it) {
        f(*it);
    }
}