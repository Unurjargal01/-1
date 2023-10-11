#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
bool Comp(std::pair<std::string, std::string> a, std::pair<std::string, std::string> b) {
    return a.first < b.first;
}
class StaticMap {
public:
    std::vector<std::pair<std::string, std::string>> data;
    explicit StaticMap(const std::vector<std::pair<std::string, std::string>>& items)
        : data(items) {
        std::sort(data.begin(), data.end(), Comp);
    }

    bool Find(const std::string& key, std::string* value) const {
        auto it = std::lower_bound(
            data.begin(), data.end(), key,
            [](std::pair<std::string, std::string> p, std::string val) { return p.first < val; });
        if (it != data.end() && it->first == key) {
            *value = it->second;
            return true;
        }
        return false;
    }
};
