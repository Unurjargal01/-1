#pragma once

#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <map>

std::vector<std::vector<int>> GeneratePermutations(size_t len) {
    std::vector<std::vector<int>> ans;
    std::map<std::vector<int>, int> ans1;
    if (len == 0) {
        return ans;
    } else if (len == 1) {
        ans.push_back({0});
        return ans;
    }

    std::vector<std::vector<int>> cur = GeneratePermutations(len - 1);
    for (auto v : cur) {
        std::vector<int> u = v;
        u.push_back(len - 1);
        size_t prev = len - 1;
        for (size_t i = 0; i < len; ++i) {
            std::swap(u[i], u[prev]);
            prev = i;
            ans1.insert(std::pair<std::vector<int>, int>(u, 1));
        }
    }

    for (std::map<std::vector<int>, int>::iterator it = ans1.begin(); it != ans1.end(); ++it) {
        ans.push_back(it->first);
    }

    return ans;
}