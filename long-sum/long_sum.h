#pragma once

#include <string>
#include <stdexcept>
#include <algorithm>

std::string LongSum(const std::string& a, const std::string& b) {
    int a_size = a.size() - 1, b_size = b.size() - 1, rem = 0;
    std::string ans;
    if (a.empty()) {
        return b;
    } else if (b.empty()) {
        return a;
    }
    while (a_size >= 0 && b_size >= 0) {
        char c = a[a_size] + b[b_size] - 48 + rem;
        rem = 0;
        if (c >= 58) {
            ++rem;
            c -= 10;
        }
        ans.push_back(c);
        --a_size;
        --b_size;
    }

    std::reverse(ans.begin(), ans.end());
    std::string ans1;
    std::string ch;
    if (rem > 0) {
        ch = "1";
    }
    if (a_size >= 0) {
        ans1 = a.substr(0, a_size + 1);
    } else if (b_size >= 0) {
        ans1 = b.substr(0, b_size + 1);
    }

    ans1 = LongSum(ans1, ch);
    ans1 += ans;

    return ans1;
}
