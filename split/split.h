#pragma once

#include <string>
#include <stdexcept>
#include <vector>

std::vector<std::string> Split(const std::string& string, const std::string& delimiter = " ") {
    std::vector<std::string> ans;
    std::string::size_type prev = 0, cur = string.find(delimiter);
    if (string.empty()) {
        return ans;
    }
    while (cur != std::string::npos) {
        ans.push_back(string.substr(prev, cur - prev));
        prev = cur + delimiter.size();
        cur = string.find(delimiter, prev);
    }
    ans.push_back(string.substr(prev, string.size()));
    return ans;
}
