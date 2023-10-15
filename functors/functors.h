#pragma once

#include <cstddef>
#include <ranges>
#include <algorithm>
template <class Functor>
class ReverseUnaryFunctor {
public:
    ReverseUnaryFunctor(Functor func) : func_(func){};
    bool operator()(auto a) {
        return !func_(a);
    }

private:
    Functor func_;
};

template <class Functor>
class ReverseBinaryFunctor {
public:
    ReverseBinaryFunctor(Functor func) : func_(func){};
    bool operator()(auto a, auto b) {
        return func_(b, a);
    }

private:
    Functor func_;
};

auto MakeReverseUnaryFunctor(auto functor) {
    return ReverseUnaryFunctor{functor};
}

auto MakeReverseBinaryFunctor(auto functor) {
    return ReverseBinaryFunctor{functor};
}

template <class Iterator>
size_t ComparisonsCount(Iterator first, Iterator last) {
    size_t ans = 0;
    std::sort(first, last, [ans](auto a, auto b) mutable {
        ++ans;
        return a < b;
    });
    return ans;
}
