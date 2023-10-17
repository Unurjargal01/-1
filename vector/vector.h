#pragma once

#include <iterator>
#include <cstddef>
#include <cmath>
#include <initializer_list>
#include <utility>
class Vector {
public:
    Vector() : data_(nullptr), size_(0), capacity_(0){};  // Constructor by default;
    Vector(const size_t& size) : data_(new int[size]), size_(size), capacity_(size_) {
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = 0;
        }
    };
    Vector(std::initializer_list<int> v)
        : data_(new int[v.size()]), size_(v.size()), capacity_(v.size()) {
        std::copy(v.begin(), v.end(), data_);
    }

    Vector(const Vector& other)
        : data_(new int[other.size_]), size_(other.size_), capacity_(other.capacity_) {
        // Copy the elements
        std::copy(other.data_, other.data_ + other.size_, data_);
    };
    Vector(Vector&& other) : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    };

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            Swap(temp);
        }
        return *this;
    }
    Vector& operator=(Vector&& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        return *this;
    };

    ~Vector() {
        delete[] data_;
        data_ = nullptr;
    }
    void Swap(Vector& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
    int operator[](size_t k) const {
        return data_[k];
    }
    int& operator[](size_t k) {
        return data_[k];
    }
    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return capacity_;
    }
    void PushBack(int elem) {
        if (size_ == capacity_) {
            if (capacity_ == 0) {
                capacity_ = 1;
            } else {
                capacity_ *= 2;
            }
            Reallocation(capacity_);
        }
        data_[size_] = elem;
        size_ += 1;
    }

    void PopBack() {
        size_ -= 1;
    }
    void Clear() {
        size_ = 0;
    }
    void Reserve(size_t capacity) {
        if (capacity_ < capacity) {
            Reallocation(capacity);
            capacity_ = capacity;
        }
    }
    void Reallocation(size_t capacity) {
        int* temp = new int[capacity];
        std::copy(data_, data_ + size_, temp);
        delete[] data_;
        data_ = temp;
    }
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = int;
        using difference_type = ptrdiff_t;
        using pointer = int*;
        using reference = int&;
        pointer ptr_;
        Iterator() : ptr_(nullptr){};
        Iterator(pointer ptr) : ptr_(ptr){};

        reference operator*() const {
            return *ptr_;
        }
        pointer operator->() const {
            return ptr_;
        }

        Iterator& operator++() {
            ++ptr_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator old = *this;
            ++ptr_;
            return old;
        }
        Iterator& operator--() {
            --ptr_;
            return *this;
        }
        Iterator operator--(int) {
            Iterator old = *this;
            --ptr_;
            return old;
        }

        Iterator& operator+=(difference_type n) {
            ptr_ += n;
            return *this;
        }
        Iterator operator+(difference_type n) const {
            Iterator temp = *this;
            return temp += n;
        }

        Iterator& operator-=(difference_type n) {
            return *this += -n;
        }

        Iterator operator-(const difference_type n) const {
            return Iterator(ptr_ - n);
        }

        friend Iterator operator+(difference_type n, const Iterator& it) {
            return Iterator(it.ptr_ + n);
        }
        friend Iterator operator-(difference_type n, const Iterator& it) {
            return Iterator(it.ptr_ - n);
        }
        friend difference_type operator-(const Iterator lhs, const Iterator rhs) {
            return lhs.ptr_ - rhs.ptr_;
        }
        reference operator[](difference_type n) const {
            return *(ptr_ + n);
        }

        auto operator<=>(const Iterator&) const = default;
    };

    Iterator begin() {
        return Iterator(data_);
    }
    Iterator end() {
        return Iterator(data_ + size_);
    }

private:
    int* data_;
    size_t size_;
    size_t capacity_;
};