#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
template <class T>
std::vector<std::unique_ptr<T>> Duplicate(const std::vector<std::shared_ptr<T>>& items) {
    // Same value but different pointer;
    std::vector<std::unique_ptr<T>> ans;
    for (auto& i : items) {
        ans.push_back(std::make_unique<T>(*i));
    }
    return ans;
};

template <class T>
std::vector<std::shared_ptr<T>> DeDuplicate(const std::vector<std::unique_ptr<T>>& items) {
    std::unordered_map<T, std::shared_ptr<T>> map;
    std::vector<std::shared_ptr<T>> ans;
    for (const auto& i : items) {
        if (map.find(*i) == map.end()) {
            std::shared_ptr<T> shared = std::make_shared<T>(*i);
            map[*shared] = shared;
        }
        ans.push_back(map[*i]);
    }
    return ans;
};