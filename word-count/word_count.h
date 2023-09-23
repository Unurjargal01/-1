#pragma once

#include <string>
#include <stdexcept>
#include <unordered_set>
#include <ctype.h>

int DifferentWordsCount(const std::string& string) {
    std::unordered_set<std::string> words;
    std::string word;
    for (auto c : string) {
        c = std::tolower(c);
        if (std::isalpha(c)) {
            word.push_back(c);
        } else if (!word.empty()) {
            words.insert(word);
            word = "";
        }
    }
    if (!word.empty()) {
        words.insert(word);
    }
    return words.size();
}
