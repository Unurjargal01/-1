#pragma once

#include <stdexcept>

int64_t Multiply(int a, int b) {
    int64_t a_64 = int64_t{a};
    int64_t b_64 = int64_t{b};
    return a_64 * b_64;
}
