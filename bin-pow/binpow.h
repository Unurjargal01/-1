#pragma once

#include <stdexcept>

int BinPow(int a, int64_t b, int c) {
    int64_t ans = 1, mov = a, n = b;
    while (n >= 1) {
        if (n % 2) {
            ans *= mov;
            ans %= c;
        }
        mov = (mov * mov) % c;
        n /= 2;
    }
    return ans;
}
