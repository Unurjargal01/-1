#pragma once

#include <cstddef>
#include <vector>
class Stack {
public:
    void Push(int x) {
        elems_.push_back(x);
    }

    bool Pop() {
        bool ans = !elems_.empty();
        elems_.pop_back();
        return ans;
    }

    int Top() const {
        return elems_.back();
    }

    bool Empty() const {
        return elems_.empty();
    }

    size_t Size() const {
        return elems_.size();
    }

private:
    std::vector<int> elems_;
};
