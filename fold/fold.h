#pragma once

#include <vector>
#include <cstddef>

struct Sum {
    Sum(){};
    int operator()(int a, int b) {
        return a += b;
    }
};

struct Prod {
    Prod(){};
    int operator()(int a, int b) {
        return a *= b;
    }
};

struct Concat {
    Concat(){};
    auto operator()(auto a, auto b) {
        a.insert(a.end(), b.begin(), b.end());
        return a;
    }
};
class Length {
public:
    Length(size_t* cnt) : ans_(cnt){};
    auto operator()(auto a, auto) {
        ++(*ans_);
        return a;
    }

private:
    size_t* ans_;
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    for (; first != last; ++first) {
        init = func(init, *first);
    }
    return init;
}
