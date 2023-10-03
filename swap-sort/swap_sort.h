#pragma once

#include <stdexcept>

void Swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void Sort3(int* a, int* b, int* c) {
    if (*a > *b) {
        Swap(a, b);
    }
    if (*c < *b) {
        Swap(b, c);
    }
    if (*a > *b) {
        Swap(a, b);
    }
}
