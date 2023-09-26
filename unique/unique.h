#pragma once

#include <vector>
#include <stdexcept>
#include <map>

std::vector<int> Unique(const std::vector<int>& data) {
    std::vector<int> ans;
    std::map<int, int> map;
    for (int i : data) {
        map.insert(std::pair<int, int>(i, 1));
    }
    for (std::map<int, int>::iterator it = map.begin(); it != map.end(); ++it) {
        ans.push_back(it->first);
    }

    return ans;
}