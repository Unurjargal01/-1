#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

std::unordered_map<int, std::string> ReverseMap(const std::unordered_map<std::string, int>& map) {
    std::unordered_map<int, std::string> ans;
    for (const auto& v : map) {
        ans[v.second] = v.first;
    }
    return ans;
}
