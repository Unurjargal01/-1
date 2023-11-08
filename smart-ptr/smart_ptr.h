#pragma once

#include <string>
struct Cnt {
    int strong;
    int weak;
};

class WeakPtr;

class SharedPtr {
    friend WeakPtr;

public:
    SharedPtr() : data_(nullptr), cnt_(new Cnt{1, 0}){};
    SharedPtr(std::string* ptr) : data_(ptr), cnt_(new Cnt{1, 0}){};
    SharedPtr(const SharedPtr& other) : data_(other.data_), cnt_(other.cnt_) {
        ++cnt_->strong;
    };
    SharedPtr(SharedPtr&& other) : data_(other.data_), cnt_(other.cnt_) {
        // data_ = std::move(other.data_);
        // cnt_ = std::move(other.cnt_);
        other.data_ = nullptr;
        other.cnt_ = nullptr;
    }
    SharedPtr(const WeakPtr& ptr);
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            Reduce();
            data_ = other.data_;
            cnt_ = other.cnt_;
            ++cnt_->strong;
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) {
        if (this != &other) {
            Reduce();
            // data_ = std::move(other.data_);
            // cnt_ = std::move(other.cnt_);
            data_ = other.data_;
            cnt_ = other.cnt_;
            other.data_ = nullptr;
            other.cnt_ = nullptr;
        }
        return *this;
    }

    ~SharedPtr() {
        Reduce();
    };

    void Reduce() {
        if (cnt_ == nullptr) {
            return;
        }
        if (--cnt_->strong == 0) {
            delete data_;
            data_ = nullptr;
            if (cnt_->weak == 0) {
                delete cnt_;
                cnt_ = nullptr;
            }
        }
    };

    std::string& operator*() {
        return *data_;
    }

    std::string& operator*() const {
        return *data_;
    }

    std::string* operator->() {
        return data_;
    }

    std::string* Get() {
        return data_;
    }

    std::string* Get() const {
        return data_;
    }

    void Reset(std::string* ptr) {
        Reduce();
        data_ = ptr;
        cnt_ = new Cnt{1, 0};
    }

private:
    std::string* data_;
    Cnt* cnt_;
};

class WeakPtr {
    friend SharedPtr;

public:
    WeakPtr() : data_(nullptr), cnt_{new Cnt{0, 1}} {};
    WeakPtr(const WeakPtr& other) : data_(other.data_), cnt_(other.cnt_) {
        ++cnt_->weak;
    };
    WeakPtr(WeakPtr&& other) : data_(other.data_), cnt_(other.cnt_) {
        // data_ = std::move(other.data_);
        // cnt_ = std::move(other.cnt_);
        other.data_ = nullptr;
        other.cnt_ = nullptr;
    };
    WeakPtr(const SharedPtr& other) : data_(other.data_), cnt_(other.cnt_) {
        ++cnt_->weak;
    }
    // WeakPtr(SharedPtr&& other) {}
    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            Reduce();
            data_ = other.data_;
            cnt_ = other.cnt_;
            ++cnt_->weak;
        }
        return *this;
    };
    WeakPtr& operator=(WeakPtr&& other) {
        if (this != &other) {
            Reduce();
            // data_ = std::move(other.data_);
            // cnt_ = std::move(other.cnt_);
            data_ = other.data_;
            cnt_ = other.cnt_;
            other.data_ = nullptr;
            other.cnt_ = nullptr;
        }
        return *this;
    };

    WeakPtr& operator=(const SharedPtr& other) {
        Reduce();
        data_ = other.data_;
        cnt_ = other.cnt_;
        ++cnt_->weak;

        return *this;
    }
    // WeakPtr& operator=(SharedPtr&& other) {}

    ~WeakPtr() {
        Reduce();
    };

    void Reduce() {
        if (cnt_ == nullptr) {
            return;
        }
        --cnt_->weak;
        if (cnt_->weak == 0 && cnt_->strong == 0) {
            delete cnt_;
            cnt_ = nullptr;
        }
    }
    SharedPtr Lock() {
        SharedPtr shared;
        if (cnt_->strong > 0) {
            shared.data_ = data_;
            delete shared.cnt_;
            shared.cnt_ = cnt_;
            ++cnt_->strong;
        }
        return shared;
    };

    bool IsExpired() const {
        return cnt_ == nullptr || cnt_->strong == 0;
    }

private:
    std::string* data_;
    Cnt* cnt_;
};

SharedPtr::SharedPtr(const WeakPtr& ptr) {
    if (ptr.cnt_->strong > 0) {
        data_ = ptr.data_;
        cnt_ = ptr.cnt_;
        ++cnt_->strong;
    } else {
        data_ = nullptr;
        cnt_ = ptr.cnt_;
        ++ptr.cnt_->strong;
    }
};
