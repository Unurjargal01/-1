#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <stack>
#include <stdlib.h>

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

int EvaluateExpression(const std::string& expression) {
    std::vector<std::string> v{Split(expression)};
    std::stack<int> stack;
    std::string op = "+ * -";
    int m, n, temp;
    for (auto i : v) {
        if (i == "+" || i == "*" || i == "-") {
            n = stack.top();
            stack.pop();
            m = stack.top();
            stack.pop();
            if (i == "+") {
                temp = m + n;
            } else if (i == "-") {
                temp = m - n;
            } else {
                temp = m * n;
            }
            stack.push(temp);
        } else {
            stack.push(std::stoi(i));
        }
    }
    return stack.top();
}