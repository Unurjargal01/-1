#pragma once

#include <cstddef>
#include <vector>
// Implementation without LinkedList
class RingBuffer {
public:
    explicit RingBuffer(size_t capacity) : ring_(capacity), cap_(capacity) {
    }

    size_t Size() const {
        return size_;
    }

    bool Empty() const {
        return size_ == 0;
    }

    bool TryPush(int element) {
        if (cap_ == size_) {
            return false;
        }
        ++size_;
        ring_[end_] = element;
        end_ = (end_ + 1) % cap_;
        return true;
    }

    bool TryPop(int* element) {
        if (!size_) {
            return false;
        }
        *element = ring_[start_];
        --size_;
        start_ = (start_ + 1) % cap_;
        return true;
    }

private:
    std::vector<int> ring_;
    size_t cap_, size_ = 0, start_ = 0, end_ = 0;
};
